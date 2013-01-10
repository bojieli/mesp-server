from mongoengine import *
from mesp.settings import DBNAME

connect(DBNAME)

class DataEntry(DynamicEmbeddedDocument):
    sn = IntField() # serial number
    DATA_SOURCES = (1, # pedometer
                    2, # temperature
                    3, # humidity
                    4, # ultraviolet
                   )
    source = IntField(choices=DATA_SOURCES)
    # here goes the dynamic fields
    meta = {
        'indexes': ['sn']
    }

class Account(Document):
    ACCOUNT_SOURCES = (1, # local admin
                       2, # renren
                       3, # sina weibo
                       4, # tencent weibo
                      )
    source = IntField(choices=ACCOUNT_SOURCES)
    username = StringField()
    next_sn = IntField(default=0)
    meta = {
        'indexes': [('source', 'username')]
    }

class AppInstance(Document):
    account = ReferenceField(Account)
    api_token = StringField()
    APP_TYPES = (1, # windows desktop
                 2, # Android
                 3, # iOS
                )
    app_type = IntField(choices=APP_TYPES)
    register_time = DateTimeField()
    token = StringField()
    data = ListField(EmbeddedDocumentField(DataEntry))
    meta = {
        'indexes': ['token']
    }

class AccessLog(Document):
    token = StringField()
    action = StringField()
    time = DateTimeField()
    ip = IntField()

