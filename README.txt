1����������
������̬��: curl jsoncpp boost_system boost_thread  (��lib�ļ�����)
������̬��: ssl crypto rt z  (��Ҫ��װ)
(1)��װopenssl�Ŀ��ͷ�ļ� http://www.openssl.org/source/ 
(2)��װcurl�Ŀ��ͷ�ļ� http://curl.haxx.se/download/curl-7.43.0.tar.gz 
(3)��װjsoncpp�Ŀ��ͷ�ļ� https://github.com/open-source-parsers/jsoncpp 
(4)��װboost�Ŀ��ͷ�ļ� http://www.boost.org/ 
(5)��װcmake���� http://www.cmake.org/download/ 

2�����ر���˵����
�޸�CMakeList.txt�ļ��У�ָ������boostͷ�ļ�·�����޸�������䣺
SET(BOOST_HEADER_DIR "/root/boost_1_61_0")

3�������ļ�˵��
"Region":"sh",                    //����COS�����ϴ����ز�����URL����ò����й�
"SignExpiredTime":360,            //ǩ����ʱʱ�䣬��λ����
"CurlConnectTimeoutInms":10000,   //CURL���ӳ�ʱʱ�䣬��λ������
"CurlGlobalConnectTimeoutInms":360000, //CURL����ִ�����ʱ�䣬��λ������
"UploadSliceSize":1048576,        //��Ƭ��С����λ���ֽڣ���ѡ����512k,1M,2M,3M(��Ҫ����ɶ�Ӧ�ֽ���)
"IsUploadTakeSha":0,              //�ϴ��ļ�ʱ�Ƿ���ҪЯ��shaֵ
"DownloadDomainType":2,           //�����������ͣ�1: cdn, 2: cos, 3: innercos, 4: self domain
"SelfDomain":"",                  //�Զ�������
"UploadThreadPoolSize":5          //���ļ���Ƭ�ϴ��̳߳ش�С
"AsynThreadPoolSize":2            //�첽�ϴ������̳߳ش�С
"LogoutType":0                    //��ӡ�����0:�����,1:�������Ļ,2:��ӡsyslog