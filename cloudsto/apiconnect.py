class ApiConnect:
    def _connect_renren(account, token):
        return 1

    def _connect_sina_weibo(account, token):
        return 1

    def _connect_tencent_weibo(account, token):
        return 1

    def checkAccount(source, account, token):
        if source == 2:
            return _connect_renren(account, token)
        if source == 3:
            return _connect_sina_weibo(account, token)
        if source == 4:
            return _connect_tencent_weibo(account, token)
        return 0
