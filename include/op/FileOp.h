#ifndef FILE_OP_H
#define FILE_OP_H
#include <pthread.h>
#include <string>
#include "op/BaseOp.h"
#include "util/FileUtil.h"
#include "util/HttpUtil.h"
#include "util/CodecUtil.h"
#include "util/HttpSender.h"
#include "util/StringUtil.h"
#include "op/FileUploadTask.h"
#include "request/FileUploadReq.h"
#include "request/FileDownloadReq.h"
#include "request/FileUpdateReq.h"
#include "request/FileStatReq.h"
#include "request/FileDeleteReq.h"
#include "request/FileMoveReq.h"
#include "request/CosResult.h"
#include "CosParams.h"
#include "CosDefines.h"
#include "CosConfig.h"
#include "CosSysConfig.h"

using std::string;
namespace qcloud_cos{

class FileOp;

class UploadCallBackArgs{
public:
    UploadCallBackArgs() : m_code(-1), m_user_data(NULL) {}
    UploadCallBackArgs(const FileUploadReq req, string rsp)
        : m_req(req), m_rsp(rsp) , m_code(-1), m_user_data(NULL) {}
    UploadCallBackArgs(const FileUploadReq req, string rsp, int code, void* user_data)
        : m_req(req), m_rsp(rsp) , m_code(code), m_user_data(user_data) {}

    UploadCallBackArgs(const UploadCallBackArgs& arg)
        : m_req(arg.m_req) , m_rsp(arg.m_rsp),
        m_code(arg.m_code), m_user_data(arg.m_user_data) {}

    FileUploadReq m_req;
    string    m_rsp;
    int       m_code;
    void*     m_user_data; // 用户传入的参数指针
};

class DownloadCallBackArgs {
public:
    DownloadCallBackArgs(){}
    DownloadCallBackArgs(const FileDownloadReq req,
                         char* buffer, size_t buf_len,
                         size_t data_size, int code, string msg) : m_req(req),m_message(msg) {
        m_buffer = buffer;
        m_buffer_len = buf_len;
        m_data_size = data_size;
        m_code = code;
        m_user_data = NULL;
    }

    DownloadCallBackArgs(const FileDownloadReq req,
                         char* buffer, size_t buf_len,
                         size_t data_size, int code, string msg,
                         void* user_data) : m_req(req), m_message(msg) {
        m_buffer = buffer;
        m_buffer_len = buf_len;
        m_data_size = data_size;
        m_code = code;
        m_user_data = user_data;
    }

    DownloadCallBackArgs(const DownloadCallBackArgs& arg): m_req(arg.m_req), m_message(arg.m_message){
        m_buffer = arg.m_buffer;
        m_buffer_len = arg.m_buffer_len;
        m_data_size = arg.m_data_size;
        m_code = arg.m_code;
        m_user_data = arg.m_user_data;
    }

    FileDownloadReq m_req; //客户端传入的请求消息
    char*  m_buffer;    //客户端传入的保存数据的buffer
    size_t m_buffer_len;//客户端传入的预读取数据的大小
    size_t m_data_size; //实际读取的数据大小
    int    m_code;      //异步下载返回码,0:成功,非0:失败
    string m_message;   //下载失败的描述
    void*  m_user_data; // 用户传入的参数指针
};

class DeleteCallBackArgs{
public:
    DeleteCallBackArgs() : m_code(-1), m_user_data(NULL) {}
    DeleteCallBackArgs(const FileDeleteReq req, string rsp)
        : m_req(req), m_rsp(rsp) , m_code(-1), m_user_data(NULL) {}
    DeleteCallBackArgs(const FileDeleteReq req, string rsp, int code, void* user_data)
        : m_req(req), m_rsp(rsp) , m_code(code), m_user_data(user_data) {}

    DeleteCallBackArgs(const DeleteCallBackArgs& arg)
        : m_req(arg.m_req) , m_rsp(arg.m_rsp),
        m_code(arg.m_code), m_user_data(arg.m_user_data) {}

    FileDeleteReq m_req;
    string    m_rsp;
    int       m_code;
    void*     m_user_data; // 用户传入的参数指针
};

typedef void (*UploadCallback)(UploadCallBackArgs arg);
typedef void (*DownloadCallback)(DownloadCallBackArgs arg);
typedef void (*DeleteCallback)(DeleteCallBackArgs arg);

class Asyn_Arg {
public:
    Asyn_Arg(FileOp* op) : m_op(op){}
    Asyn_Arg(const Asyn_Arg& arg){
        this->m_op = arg.m_op;
    }
    virtual ~Asyn_Arg(){};
    FileOp* m_op;
};

class Upload_Asyn_Arg : public Asyn_Arg {
public:
    Upload_Asyn_Arg(FileOp* pFileOp,
                    const FileUploadReq& req,
                    UploadCallback cb,
                    void* user_data) : Asyn_Arg(pFileOp) , m_req(req){
        m_callback = cb;
        m_user_data = user_data;
    }

    Upload_Asyn_Arg(const Upload_Asyn_Arg& arg)
        : Asyn_Arg(arg),
          m_req(arg.m_req),
          m_callback(arg.m_callback),
          m_user_data(arg.m_user_data) {
    }

    virtual ~Upload_Asyn_Arg(){}
    FileUploadReq  m_req;
    UploadCallback m_callback;
    void* m_user_data;
};

class Download_Asyn_Arg : public Asyn_Arg {
public:
    Download_Asyn_Arg(FileOp* pFileOp,
                      const FileDownloadReq& req,
                      char* buffer, size_t bufLen,
                      DownloadCallback cb,
                      void* user_data) : Asyn_Arg(pFileOp),m_req(req) {
        m_callback = cb;
        m_buffer = buffer;
        m_bufLen = bufLen;
        m_user_data = user_data;
    }

    Download_Asyn_Arg(const Download_Asyn_Arg& arg) : Asyn_Arg(arg), m_req(arg.m_req), m_callback(arg.m_callback){
        this->m_buffer = arg.m_buffer;
        this->m_bufLen = arg.m_bufLen;
        this->m_user_data = arg.m_user_data;
    }

    virtual ~Download_Asyn_Arg(){}

    char*  m_buffer;
    size_t m_bufLen;
    FileDownloadReq  m_req;
    DownloadCallback  m_callback;
    void* m_user_data;
};

class Delete_Asyn_Arg : public Asyn_Arg {
public:
    Delete_Asyn_Arg(FileOp* pFileOp,
                    const FileDeleteReq& req,
                    DeleteCallback cb,
                    void* user_data) : Asyn_Arg(pFileOp) , m_req(req) {
        m_callback = cb;
        m_user_data = user_data;
    }

    Delete_Asyn_Arg(const Delete_Asyn_Arg& arg)
        : Asyn_Arg(arg),
          m_req(arg.m_req),
          m_callback(arg.m_callback),
          m_user_data(arg.m_user_data) {}

    virtual ~Delete_Asyn_Arg(){}
    FileDeleteReq m_req;
    DeleteCallback m_callback;
    void* m_user_data;
};

extern "C" {
void  FileDownload_Asyn_Thread(Download_Asyn_Arg arg);
void  FileUpload_Asyn_Thread(Upload_Asyn_Arg arg);
void  FileDelete_Asyn_Thread(Delete_Asyn_Arg arg);
}

class FileOp : public BaseOp {

public:
    FileOp(CosConfig& config) : BaseOp(config) {};
    ~FileOp(){};
    int    FileDownload(FileDownloadReq& req, char* buffer, size_t bufLen, uint64_t offset, int* ret_code);
    bool   FileDownloadAsyn(FileOp& op,
                            const FileDownloadReq& req,
                            char* buffer, size_t bufLen,
                            DownloadCallback callback, void* user_data);
    string FileUpload(FileUploadReq& req);
    bool   FileUploadAsyn(FileOp& op,
                          const FileUploadReq& req,
                          UploadCallback callback,
                          void* user_data);
    string FileUpdate(FileUpdateReq& req);
    string FileStat(FileStatReq& req);
    string FileDelete(FileDeleteReq& req);
    bool FileDeleteAsyn(FileOp& op,
                        const FileDeleteReq& req,
                        DeleteCallback callback,
                        void* user_data);
    string FileMove(FileMoveReq& req);
    string FileUploadSliceList(FileUploadReq& req);
    string FileUploadSingle(FileUploadReq& req);
    string FileUploadSlice(FileUploadReq& req);

private:
    FileOp();
    string FileUploadSliceFinish(FileUploadReq& req, string& dataRspJson);
    string FileUploadSliceData(FileUploadReq& req, string& initRsp);
    string FileUploadSliceInit(FileUploadReq& req);
};

}


#endif
