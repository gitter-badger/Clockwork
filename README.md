![Clockwork logo](https://raw.githubusercontent.com/clockwork/Clockwork/master/bin/Data/Textures/LogoLarge.png)

#Clockwork

**Clockwork** is a free lightweight, cross-platform 2D and 3D game engine implemented in C++ and released under the MIT license. Greatly inspired by OGRE and Horde3D.

Main website: [http://clockwork.sunwells.net/](http://clockwork.sunwells.net/)

##License
Licensed under the MIT license, see [License.txt](https://github.com/clockwork/Clockwork/blob/master/License.txt) for details.

##Credits
Clockwork is greatly inspired by OGRE (http://www.ogre3d.org) and Horde3D
(http://www.horde3d.org). Additional inspiration & research used:
- Rectangle packing by Jukka Jylänki (clb)
  http://clb.demon.fi/projects/rectangle-bin-packing
- Tangent generation from Terathon
  http://www.terathon.com/code/tangent.html
- Fast, Minimum Storage Ray/Triangle Intersection by Möller & Trumbore
  http://www.graphics.cornell.edu/pubs/1997/MT97.pdf
- Linear-Speed Vertex Cache Optimisation by Tom Forsyth
  http://home.comcast.net/~tom_forsyth/papers/fast_vert_cache_opt.html
- Software rasterization of triangles based on Chris Hecker's
  Perspective Texture Mapping series in the Game Developer magazine
  http://chrishecker.com/Miscellaneous_Technical_Articles
- Networked Physics by Glenn Fiedler
  http://gafferongames.com/game-physics/networked-physics/
- Euler Angle Formulas by David Eberly
  http://www.geometrictools.com/Documentation/EulerAngles.pdf
- Red Black Trees by Julienne Walker
  http://eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx
- Comparison of several sorting algorithms by Juha Nieminen
  http://warp.povusers.org/SortComparison/

Clockwork uses the following third-party libraries:
- AngelScript 2.29.1 (http://www.angelcode.com/angelscript/)
- Box2D 2.3.0 (http://box2d.org/)
- Bullet 2.82 (http://www.bulletphysics.org/)
- Civetweb (http://sourceforge.net/projects/civetweb/)
- FreeType 2.5.0 (http://www.freetype.org/)
- GLEW 1.9.0 (http://glew.sourceforge.net/)
- jo_jpeg 1.52 (http://www.jonolick.com/uploads/7/9/2/1/7921194/jo_jpeg.cpp)
- kNet (https://github.com/juj/kNet)
- libcpuid 0.2.0 (http://libcpuid.sourceforge.net/)
- Lua 5.1 (http://www.lua.org)
- LuaJIT 2.0.3 (http://www.luajit.org)
- LZ4 (http://code.google.com/p/lz4/)
- MojoShader (http://icculus.org/mojoshader/)
- nanodbc 2.2.2 (http://lexicalunit.github.io/nanodbc/)
- Open Asset Import Library (http://assimp.sourceforge.net/)
- pugixml 1.5 (http://pugixml.org/)
- rapidjson 0.11 (https://code.google.com/p/rapidjson/)
- Recast/Detour (https://github.com/memononen/recastnavigation/)
- SDL 2.0.3 (http://www.libsdl.org/)
- StanHull (http://codesuppository.blogspot.com/2006/03/
  john-ratcliffs-code-suppository-blog.html)
- stb_image 2.05 (http://nothings.org/)
- stb_image_write 0.98 (http://nothings.org/)
- stb_vorbis 1.05 (http://nothings.org/)
- SQLite 3.8.10.2 (https://www.sqlite.org/)
- tolua++ 1.0.93 (http://www.codenix.com/~tolua)

DXT / ETC1 / PVRTC decompression code based on the Squish library and the Oolong
Engine.
Jack and mushroom models from the realXtend project. (http://www.realxtend.org)
Ninja model and terrain, water, smoke, flare and status bar textures from OGRE.
BlueHighway font from Larabie Fonts.
Anonymous Pro font by Mark Simonson.
NinjaSnowWar sounds by Veli-Pekka Tätilä.

##Documentation
Clockwork classes have been sparsely documented using Doxygen notation. To
generate documentation into the "Docs" subdirectory, open the Doxyfile in the
"Docs" subdirectory with doxywizard and click "Run doxygen" from the "Run" tab.
Get Doxygen from http://www.doxygen.org & Graphviz from http://www.graphviz.org.
See section "Documentation build" below on how to automate documentation
generation as part of the build process.

The documentation is also available online at
  http://docs.sunwells.net

Documentation on how to build Clockwork:
  http://docs.sunwells.net/_building.html
Documentation on how to use Clockwork as external library
  http://docs.sunwells.net/_using_library.html

Replace HEAD with a specific release version in the above links to obtain the
documentation pertinent to the specified release. Alternatively, use the
document-switcher in the documentation website to do so.

##History
The change history is available online at
  http://docs.sunwells.net/_history.html
