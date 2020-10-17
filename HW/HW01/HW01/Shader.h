//
//  Shader.h
//  Test
//
//  Created by 陳柏丞 on 2020/10/12.
//

#ifndef Shader_h
#define Shader_h


#endif /* Shader_h */

#include <string>

using namespace std;

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    
    string vertexString;
    string fragmentString;
    const char* vertexSource;
    const char* fragmentSource;
    
    unsigned int ID; // Shader Porgram ID
    void use();
    
//    ~Shader();
    
private:
    void checkCompilerErrors(unsigned int ID, string type);
};
