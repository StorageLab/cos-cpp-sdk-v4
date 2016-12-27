#include "CosApi.h"
#include "CosConfig.h"
#include "CosParams.h"
#include "CosDefines.h"
#include "CosSysConfig.h"
#include "util/StringUtil.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>

using std::string;
using std::cout;
using std::endl;
using namespace qcloud_cos;

extern "C" {
//异步下载文件的回调函数
void download_callback(DownloadCallBackArgs arg)
{
    //请求消息是客户端传入的,客户端不用再次释放
    static int i = 0;
    SDK_LOG_DBG("download callback...%d", i++);
    return;
}

//异步上传文件的回调函数
void upload_callback(UploadCallBackArgs arg)
{
    string rsp = arg.m_rsp;

    SDK_LOG_DBG("upload callback...rsp:%s",rsp.c_str());
    return;
}

struct TestData {
    std::string m_name;
    int m_age;
    TestData(const std::string& name, int age) : m_name(name), m_age(age) {}

    std::string ToString() {
        std::ostringstream oss;
        oss << "name: " << m_name << ", age: " << m_age;
        return oss.str();
    }
};

void upload_callback_with_user_data(UploadCallBackArgs arg) {
    string rsp = arg.m_rsp;

    SDK_LOG_DBG("upload callback with user_data ...rsp:%s, code:%d",rsp.c_str(), arg.m_code);
    if (arg.m_user_data != NULL) {
        TestData* tmp = static_cast<TestData*>(arg.m_user_data);
        SDK_LOG_DBG("upload callback with user_data ...user_data:%s", tmp->ToString().c_str());
    }
    return;
}

void delete_callback(DeleteCallBackArgs arg)
{
    string rsp = arg.m_rsp;

    SDK_LOG_DBG("Delete callback...rsp:%s",rsp.c_str());
    return;
}

}

int main()
{
    string bucket = "jonnxu00";
    string srcpath = "./testdata/test1";
    string srcpath2 = "./testdata/test2";
    string dstpath = "/test1";
    string dstpath2 = "/test2";
    string folder = "/testdata/";
    string folder_biz_attr = "folder attribute";
    /*设置cos系统参数*/

    //生成CosAPI对象
    CosConfig config("config.json");
    CosAPI cos(config);

    FileDownloadReq fileDownloadReq(bucket,dstpath);
    int ret_code;
    uint64_t size = cos.FileDownload(fileDownloadReq, "./aaa", &ret_code);
    cout << "size =" << size << std::endl;

/*
    //从本地文件中上传文件(异步上传)
    FileUploadReq fileUploadReq1(bucket,srcpath, dstpath);
    //设置允许同名文件覆盖
    fileUploadReq1.setInsertOnly(0);
    cos.FileUploadAsyn(fileUploadReq1, upload_callback);

    FileUploadReq fileUploadReq2(bucket,srcpath2, dstpath2);
    fileUploadReq2.setInsertOnly(0);
    TestData test_data("userliu", 10);
    cos.FileUploadAsyn(fileUploadReq2, upload_callback_with_user_data, &test_data);

    sleep(1);

    char buf[1024];

    FileDownloadReq fileDownloadReq(bucket,dstpath);
    int ret_code;
    uint64_t size = cos.FileDownload(fileDownloadReq, buf, 1024, 100, &ret_code);

    cos.FileDownloadAsyn(fileDownloadReq, buf, 1024, download_callback);

    char buf2[1024];
    FileDownloadReq fileDownloadReq2(bucket,dstpath2);
    bool f2 = cos.FileDownloadAsyn(fileDownloadReq2,buf2,1024, download_callback);
    cout << f2 << endl;

    string ret;
    //目录创建
    FolderCreateReq folderCreateReq(bucket,folder,folder_biz_attr);
    ret = cos.FolderCreate(folderCreateReq);
    cout << ret <<endl;

    //目录更新
    folder_biz_attr = "folder new_attribute";
    FolderUpdateReq folderUpdateReq(bucket,folder, folder_biz_attr);
    ret = cos.FolderUpdate(folderUpdateReq);
    cout << ret <<endl;

    //目录查询
    FolderStatReq folderStatReq(bucket,folder);
    ret = cos.FolderStat(folderStatReq);
    cout << ret <<endl;

    //从本地文件中上传文件
    FileUploadReq fileUploadReq(bucket,srcpath, dstpath);
    ret = cos.FileUpload(fileUploadReq);
    cout << ret <<endl;

    //从buffer中上传文件(从内存上传暂时只支持小于8M的)
    string testbuf(1024,'A');
    FileUploadReq fileBufUploadReq(bucket,dstpath,testbuf.c_str(), testbuf.length());
    fileBufUploadReq.setInsertOnly(0);
    ret = cos.FileUpload(fileBufUploadReq);
    cout << ret <<endl;

    //文件更新
    map<string, string> custom_header;
    custom_header[PARA_CACHE_CONTROL] = "PARA_CACHE_CONTROL";
    custom_header[PARA_CONTENT_TYPE] = "PARA_CONTENT_TYPE";
    custom_header[PARA_CONTENT_DISPOSITION] = "PARA_CONTENT_DISPOSITION";
    custom_header[PARA_CONTENT_LANGUAGE] = "PARA_CONTENT_LANGUAGE";
    custom_header[PARA_X_COS_META_PREFIX + "abc"] = "PARA_X_COS_META_PREFIX";
    FileUpdateReq fileUpdateReq(bucket,dstpath);
    string file_biz_attr = "file new_attribute";
    string auth = "eInvalid";
    fileUpdateReq.setBizAttr(file_biz_attr);
    fileUpdateReq.setAuthority(auth);
    fileUpdateReq.setForbid(0);
    fileUpdateReq.setCustomHeader(custom_header);
    ret = cos.FileUpdate(fileUpdateReq);
    cout << ret <<endl;

    //文件查询
    FileStatReq fileStatReq(bucket,dstpath);
    ret = cos.FileStat(fileStatReq);
    cout << ret <<endl;

    //目录列表
    FolderListReq folderListReq(bucket,folder);
    ret = cos.FolderList(folderListReq);
    cout << ret <<endl;

    //文件删除
    FileDeleteReq fileDeleteReq(bucket, dstpath);
    ret = cos.FileDelete(fileDeleteReq);
    cout << ret <<endl;
    // 异步删除
    FileDeleteReq fileDeleteReq2(bucket, dstpath2);
    ret = cos.FileDeleteAsyn(fileDeleteReq2, delete_callback);
    sleep(1);

    //目录删除
    FolderDeleteReq folderDeleteReq(bucket, folder);
    ret = cos.FolderDelete(folderDeleteReq);
    cout << ret <<endl;
*/
    return 0;
}

