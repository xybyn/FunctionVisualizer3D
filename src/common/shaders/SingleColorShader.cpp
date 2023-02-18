#include "SingleColorShader.h"

void SingleColorShader::use()
{
    DefaultShader::use();
    setVec3("color", color);
}
