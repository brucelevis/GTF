
#include "gtf/RHI.h"

#include <memory>

class BuiltInGfxData
{
public:
	//call in main window constructor
	void init();

public:
	std::unique_ptr<gtf::RHIShaderProgram> renderMeshProgram;
};