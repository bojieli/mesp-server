from cloudsto.models import *
from django.http import HttpResponse
import base64
import random
from cloudsto.apiconnect import ApiConnect
from cloudsto.binstr import binstr
from datetime import datetime

def _token_gen(size, chars=string_ascii_uppercase + string.ascii_lowercase + string.digits):
    return ''.join(random.choice(chars) for x in range(size))

def index(request):
    return HttpResponse(chr(0)) # status OK

# Request:
#   app_type | source |   username    |   app_token
#       1    |    1   | length(1) str | length(1) str

# Response:
#   status |    token
#      1   | length(1) str

def register(request):
    try:
        req = binstr(HttpRequest.body)
        app_type = req.getbyte()
        source = req.getbyte()
        username = req.getstr()
        api_token = req.getstr()
    except:
        return HttpResponse(chr(1))
    if not ApiConnect.checkAccount(source, username, api_token):
        return HttpResponse(chr(2)) # account activation error
    TOKEN_LENGTH=20
    token = _token_gen(TOKEN_LENGTH)
    try:
        account = Account.get_or_create(source=source, username=username)   
        AppInstance(account=account,
                    app_type=app_type,
                    api_token=api_token,
                    register_time=datetime.now(), 
                    token=token
                   ).save()
    except:
        return HttpResponse(chr(3)) # save instance error
    response = binstr()
    response.putbyte(0) # status OK
    response.putstr(token)
    return HttpResponse(response.toString())

# Request:
#       token     | entry num |   entry-1  |  entry-2   | ...
#   length(1) str |     4     | source ... | source ... |

# Three types of entries:
# pedometer:
#   source | start time | end time | step count
#   chr(1) |     4      |    4     |      4
# temperature:
#   source | start time | interval | data count | data-1 | data-2 ...
#   chr(2) |     4      |    4     |      4     |   4    |   4
# humidity:
#   source | start time | interval | data count | data-1 | data-2 ...
#   chr(3) |     4      |    4     |      4     |   4    |   4
# ultraviolet:
#   source | time | data |
#   chr(4) |   4  |   4  |

# Note: times are in seconds. Start and end times are UNIX timestamp.

# Response:
#   status | end serial number
#      1   |        4

def save(request):
    try:
        req = binstr(HttpRequest.body)
        token = req.getstr()
        entry_num = req.getlong()
    except:
        return HttpResponse(chr(1)) # parse error
    app = AppInstance(token=token)
    if not app:
        return HttpResponse(chr(2)) # invalid token
    sn = app.account.next_sn
    app.account.update_one(inc__next_sn=1)
    for i in range(entry_num):
        try:
            save_entry(app, req, sn)
        except Exception as e:
            return HttpResponse(chr(e.args[0]))

    resp = binfmt()
    resp.putbyte(0) # status OK
    resp.putlong(sn)
    return HttpResponse(resp.toString())

# Request:
#       token     | sn. begin | sn. num (-1 for unlimited)
#   length(1) str |     4     |    4
# Note: sn. begin should be end_serial_number + 1

# Response:
#   status | sn. max | entry num |  entry-1   |  entry-2   | ...
#      1   |     4   |     4     | source ... | source ... |

def receive(request):
    try:
        req = binstr(HttpRequest.body)
        token = req.getstr()
        sn_begin = req.getlong()
        sn_num = req.getlong()
    except:
        return HttpResponse(chr(1)) # parse error
    app = AppInstance(token=token)
    if not app:
        return HttpResponse(chr(2)) # invalid token
    resp = binfmt()
    resp.putbyte(0) # status OK
    if sn_num == -1:
        entries = app.data.objects(sn__gte=sn_begin)
    else
        entries = app.data.objects(sn__gte=sn_begin, sn__lt=sn_begin+sn_num)
    max_sn = 0
    for entry in entries:
        if entry.sn > max_sn:
            max_sn = entry.sn
    resp.putlong(max_sn)
    resp.putlong(entries.count())
    for entry in entries:
        get_entry(resp, entry)
    return HttpResponse(resp.toString())

def save_entry(app, req, sn):
    entry = DataEntry()
    entry.sn = sn
    source = req.getbyte()
    if source == 1:
        entry.start_time = req.getlong()
        entry.end_time = req.getlong()
        entry.step_count = req.getlong()
    else if source in [2,3]:
        entry.start_time = req.getlong()
        entry.interval = req.getlong()
        count = req.getlong()
        for i in range(count):
            entry.data[i] = req.getlong()
    else if source == 4:
        entry.time = req.getlong()
        entry.data = req.getlong()
    else
        raise Exception(10) # invalid source
    try:
        entry.insert(app.data) # save to the list
    except:
        raise Exception(11) # data save error

def get_entry(resp, entry):
    resp.putlong(entry.source)
    if entry.source == 1:
        resp.putlong(entry.start_time)
        resp.putlong(entry.end_time)
        resp.putlong(entry.step_count)
    else if entry.source in [2,3]:
        resp.putlong(entry.start_time)
        resp.putlong(entry.interval)
        resp.putlong(entry.data.count())
        for d in entry.data:
            resp.putlong(d)
    else if entry.source == 4:
        resp.putlong(entry.time)
        resp.putlong(entry.data)
    else
        raise Exception(10) # internal error

