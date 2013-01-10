class ApiConnect:
    @staticmethod
    def _connect_renren(account, token):
        return 1

    @staticmethod
    def _connect_sina_weibo(account, token):
        return 1

    @staticmethod
    def _connect_tencent_weibo(account, token):
        return 1

    @staticmethod
    def checkAccount(source, account, token):
        if source == 2:
            return ApiConnect._connect_renren(account, token)
        if source == 3:
            return ApiConnect._connect_sina_weibo(account, token)
        if source == 4:
            return ApiConnect._connect_tencent_weibo(account, token)
        return 0
