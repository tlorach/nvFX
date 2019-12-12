````
    Copyright (c) 2013, NVIDIA CORPORATION. All rights reserved.
    Copyright (c) 2013, Tristan Lorach. All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Neither the name of NVIDIA CORPORATION nor the names of its
       contributors may be used to endorse or promote products derived
       from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
    PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
    OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

````

# What is nvFX ?

nvFx is a new approach for compositing shaders and compute kernels together, using an API-agnostic description of effects for objects materials and scene management (post-processing, management of rendering passes). This approach builds on the original concepts of NVIDIA CgFX and expands it to new levels of flexibility and extensibility. Rather than creating a new shading language, we supersede existing ones (GLSL, HLSL) to avoid complex parsing and yet deploy the effect in a variety of environments.
For more details, check the presentation here :

Siggraph 2013:
http://www.nvidia.com/object/siggraph2013-tech-talks.html 
( direct Ustream link http://www.ustream.tv/recorded/36222497 )

NVIDIA GTC conference:
http://www.gputechconf.com/gtcnew/on-demand-gtc.php?searchByKeyword=nvFx&searchItems=&sessionTopic=&sessionEvent=&sessionYear=&sessionFormat=&submit=&select=+#2173

and Siggraph:
http://nvidia.fullviewmedia.com/siggraph2012/ondemand/SB117.html

There is a Doxygen folder with more details. However I need to update the documentation for few topics and added features

![Example](https://github.com/tlorach/nvFX/raw/master/doxygen/DoxyPics/Watershot.png)

## Comments on the code

I tried to keep the code as simple as possible. It is difficult to stick to that because new features always added-up yet another set of code that made it more complicated than I expected.
However, I tried on purpose to not get into crazy optimizations, so that it is still fine to step into the source code without getting too much lost. In other words, I didn't design so much the inside of nvFx and left some code non-optimized for the benefit of keeping it readable (example: iteration through pass-states). I figured-out that better optimizations could be done by whoever wants to adopt the concept of nvFx; or could be done as another step or in another branch...

I am using the idea of Abstract classes as the API entry points. But I didn't push so far this sort of abstraction, to avoid *over-engineering* the whole project. So, the project is really C++ centric and thus can't be used from pure C project.

I am also aware that any Bison/Flex experts will have 'something to say' about the actual grammar... I have identified lots of things to fix and clean and will do it as soon as I have time. For now, this is working and at least is a good enough implementation.

-----------------------------------------------------------------
# To be done as soon as possible

* update Doxygen documentation
* Linux and Mac platform targets
* add CUDA examples
** add CUDA features (such as automatic grid/block computation depending on render-target size...)
* catch-up on D3D part (I prioritized OpenGL, so far. D3D is limited in its features)
* add a D3D sample
* add the OptiX implementation
 * add a sample for ray tracing
 * add sample for hybrid rendering (OpenGL + Ray tracing for reflection)
* add a sample showing various ways of efficiently update constant buffers
* add a Path rendering example (NVIDIA OpenGL Path rendering)
* add a sample project and related example on how to use it for OpenGL ES2 under Android (using Eclipse)
* add a sample project and related example on how to use it for OpenGL ES2 on iPad
* add a sample showing how to perform complex fluid dynamic through nvFx
* add a sample showing how to perform modular techniques to mix various features such as picking passes; supersampling and downsampling passes on top of the main passes

-----------------------------------------------------------------
# Known issues
* many 'warning' at compile time. Need some clean-up
* Linux and Mac platforms still under construction
* uniform parameter assignment : it might crash when assigning a type of uniform with a method made for another type. It should not happen and will be corrected
* Automatic Sampler units seem to have issues
* Performance optimizations
* the separable shader extension showed some strange behavior. To check
* Bison/Flex grammar and lexer : need to rewrite many details
* CMake doesn't allow to setup any Visual Sutdio local parameters. The Debug section (Property>>Configuration Properties>>Debugging>>Working Directory) **must** have "Working Directory" set to **$(TargetDir)**. If you don't set it, the samples might complain or fail at init time.

-----------------------------------------------------------------
# What do you need to build properly
* cmake (I used v2.8.9)
* glew (I used 1.9.0)
* glut as an *option*. If not checked in cmake, the samples will use a win32-based sample to call the functions normally used by glut
* CUDA SDK as an option
* DX-SDK is you chose to build the D3D part (limited features, for now)
* bison and Flex. If you don't provide them, the project will just use the pre-created versions of the parser/lexer files
* ZLib (used for decompressing the models of the samples)
* **SvcMFCUI** : a simple UI used by the samples to allow you to play with parameters. You can find it here: https://github.com/tlorach/SvcMFCUI
* Doxygen
* some *bk3d* binary models: I have put 2 models in the resources of the samples and this will be enough for the demos. bk3d stands for *baked 3d* models. The idea is to bake as much as possible in a binary file to lower the amount of work to parse data from the samples. I will soon make this project available on GitHub, too

For running samples, please check the comment in "Known issues" about "Working Directory"

You can install nvFx thanks to CMake. It is a clean way to use it later for other projects

# how to configure CMake
## options
* **BUILD_SAMPLES**: will add the examples in the nvFx project
* **NVFXCC_ON_EFFECTS**: by default is off. If on, the project will add some compilation directives of the effects through the cmd-line nvFx compiler. This allows to check the shader effects prior to run the samples. Not mandatory but good example on how you could do this for yourself.
* **USE_CUDA**: includes CUDA part
* **USE_D3D**: includes the D3D part
* **USE_DLLSO**: **not ready, yet !**. This allows to compile nvFx project as dlls or .so binaries, instead of simple libraries
* **USE_GLUT**: for glut lovers only ;-) Under Windows you can avoid it (at last)
* **USE_NSIGHT**: if checked, the projects will get decorated with NSight markers. Useful to record a trace and see how the runtime is working on NSight timeline
* **USE_OPENGL**: Add OpenGL part ( the most up to date section, for now)
* **USE_OPTIX**: Add OptiX part. **Not ready, yet**
* **USE_SVCUI**: Allow samples to use the dll you would have built *and installed* from https://github.com/tlorach/SvcMFCUI

## external packages
CMake will try its best to find what is required. If it failed, you can still have a look at the *cmake* folder in which you will see the scripts used to try to locate those components.

I know this is the most annoying and frustrating part, when cmake doesn't want to build a clean project. So I will do my best to add comments on possible issues.

###Glew
When cmake tries to find glew, it can become tricky. The best solutions for me was to add a trick: if glew.c is found, the project will use it instead of linking with any library. This solve lots of headache.
* I used Glew 1.9.0
* you can set an env. variable or a cmake local variable : **GLEW_LOCATION** to point to the root of glew project (C:\glew-1.9.0 for example)
* if cmake found glew.c ("${GLEW_LOCATION}\src\glew.c"), it will add it to projects, rather than linking with glew lib/dll...

### User Interface SvcMFCUI
CMake will try to locate the UI library SvcMFCUI by going back in the file system and check if you downloaded it from GitHub... *and compiled* it. It will of course check CMake variable and env. variables.
See https://github.com/tlorach/SvcMFCUI for more information

### ZLib
I used the native ZLib project and installed it (C:\Program Files\zlib) where 'include', 'lib' and 'bin' folders are.

### GLUT
GLUT_LOCATION can be set as env. variable or cmake variable to point to Freeglut root folder. But again: Glut is not mandatory...

### Bison & Flex
* You can set "BISON_EXECUTABLE" either as env. variable or cmake variable (ex.: BISON_EXECUTABLE=C:\GnuWin32\bin\bison.exe)
* You can set "FLEX_EXECUTABLE" wither as env. variable or cmake variable (ex.: FLEX_EXECUTABLE=C:\GnuWin32\bin\flex.exe"

# Major update history
* 6/20/2013 : first drop on GitHub

-----------------------------------------------------------------

Please direct any questions to tlorach@nvidia.com (Tristan Lorach)

I hope this project will be of any help for some of you!
