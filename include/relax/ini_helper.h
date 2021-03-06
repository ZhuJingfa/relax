/**
 * Relax library
 *
 * ini配置文件解析库
 * #include <relax/ini_helper.h>
 *
 * @author 祝景法
 * @email zhujingfa@gmail.com
 *
 * @since 2014/09/17
 */
#ifndef RELAX_UTILITY_INI_HELPER_H_
#define RELAX_UTILITY_INI_HELPER_H_

#include <map>
#include <string>
#include <relax/relax.h>

namespace relax {

using std::map;
using std::string;

class IniEnv;
class NodeValue;

/**
 * Ini文件解析器
 *
 * Usage:
 * IniHelper *ini;
 * Status s=IniHelper::Factory("test.ini", &ini);
 *
 * //负责清空所有分配内存
 * delete ini;
 */
class IniHelper {
public:
	const char* kEnvSeparator=":";
	const char* kAssign = "=";

    ~IniHelper();

    /**
     * 获取一个ini实例
     */
     static Status Factory(string filename, IniHelper** instance);

    /**
     * 获取一个环境的值
     */
     Status Get(string env, IniEnv** value);

     /**
	  * 获取一个全局环境的值
	  */
	  Status GetGlobalIni(IniEnv** global);

     /**
	  * 获取一个环境的值
	  */
	  Status GetOrAppend(string env, IniEnv* parent, IniEnv** value);

private:
	 const char* const kGlobalIni="__global__";
     IniHelper(string& filename);

     //禁用拷贝、赋值
     IniHelper(const IniHelper&);
     IniHelper operator=(const IniHelper&);

    /**
     * environment => IniEnv
     *
     * 不在命名空间下的是global环境，全局可用。其他通过继承关系解析。
     */
    map<string, IniEnv*> container_;
};

class IniEnv {
public:
	const char* kKeySeparator=".";
	const char* kAssign = "=";

    /**
     * 获取一个键的值
     */
     Status Get(string key, string& value);
     Status Get(string key, int& value);
     Status Get(string key, double& value);

    /**
     * 设置一个键的值
     *
     */
     Status Set(string key, string value);

     Status set_parent (IniEnv* parent){
    	 parent_=parent;
    	 return Status::GetOK();
     }
     Status parent (IniEnv** parent){
    	 *parent=parent_;
    	 return Status::GetOK();
     }

     /**
      * 导出为字符串
      */
     string ToString();
     Status ToStringMap(map<string, string>& value);

protected:
     IniEnv() : parent_(NULL) {
	 }
     explicit IniEnv(IniEnv* parent) : parent_(parent) {
     }

	 ~IniEnv();

private:
     IniEnv* parent_;
     friend class IniHelper;

     //禁用拷贝、赋值
     IniEnv(const IniEnv&);
     IniEnv operator=(const IniEnv&);

     /**
      * key => value
      *
      * 存放的是顶级值
      */
     map<string, NodeValue*> container_;
};

} //relax

#endif//RELAX_UTILITY_INI_HELPER_H_
