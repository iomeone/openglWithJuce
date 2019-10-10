/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== README.md ==================
static const unsigned char temp_binary_data_0[] =
"# Port opengl tutorial on https://learnopengl.com/ to JUCE framework!\r\n"
"\r\n"
"## how to build \r\n"
"Just open the solution file in master/Builds (current only support xcode visual studio), and there you go! If you can not build or errors come out, please let me know!\r\n"
"\r\n"
"## Tutorial1:Triangle  https://learnopengl.com/Getting-started/Hello-Triangle\r\n"
"## Tutorial2:Shader     https://learnopengl.com/Getting-started/Shaders\r\n"
"\r\n"
"![tutorial1And2.gif](https://raw.githubusercontent.com/iomeone/openglWithJuce/master/Screen/tutorial1And2.gif)  \r\n"
"\r\n"
"## Tutorial3:Texture   https://learnopengl.com/Getting-started/Textures\r\n"
"## Tutorial4:Transormations https://learnopengl.com/Getting-started/Transformations\r\n"
"\r\n"
"![openglWithJuce3And4.gif](https://raw.githubusercontent.com/iomeone/openglWithJuce/master/Screen/openglWithJuce3And4.gif)  \r\n"
"\r\n"
"## Tutorial5:Coordinate Systems  https://learnopengl.com/Getting-started/Coordinate-Systems\r\n"
"![openglWithJuce5.gif](https://raw.githubusercontent.com/iomeone/openglWithJuce/master/Screen/openglWithJuce5.gif)  \r\n"
"\r\n"
"## Tutorial6:Camera  https://learnopengl.com/Getting-started/Camera\r\n"
"![openglWithJuce6.gif](https://raw.githubusercontent.com/iomeone/openglWithJuce/master/Screen/openglWithJuce6.gif)  \r\n"
"### how pitch yaw work!\r\n"
"![pitch_yaw.png](https://raw.githubusercontent.com/iomeone/openglWithJuce/master/Screen/pitch_yaw.png)  \r\n"
"\r\n"
"## Tutorial7:Colors  https://learnopengl.com/Lighting/Colors\r\n"
"![Colors.png](https://raw.githubusercontent.com/iomeone/openglWithJuce/master/Screen/Colors.png)  \r\n"
"\r\n"
"## Tutorial8:Basic-Lighting  https://learnopengl.com/Lighting/Basic-Lighting\r\n"
"![Basic-Lighting](https://raw.githubusercontent.com/iomeone/openglWithJuce/master/Screen/Basic-Lighting.gif)  \r\n"
"\r\n"
"## 15:Model-Loading  https://learnopengl.com/Model-Loading/Model\r\n"
"![Model-Loading](https://raw.githubusercontent.com/iomeone/openglWithJuce/master/Screen/model.png)  ";

const char* README_md = (const char*) temp_binary_data_0;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x64791dc8:  numBytes = 1875; return README_md;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "README_md"
};

const char* originalFilenames[] =
{
    "README.md"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
    {
        if (namedResourceList[i] == resourceNameUTF8)
            return originalFilenames[i];
    }

    return nullptr;
}

}
