BlenderExporter

- Has moved to https://github.com/reattiva/Clockwork-Blender

OgreBatchConverter

- Contributed by Carlo Carollo. Converts multiple Ogre .mesh.xml files (also from
  subdirectories) by invoking the OgreImporter tool. Use the CMake option
  CLOCKWORK_EXTRAS to include in the build.

OgreMaxscriptExport

- Contributed by Vladimir Pobedinsky. A modified version of the Maxscript
  Exporter from the Ogre SDK that will import Ogre .mesh.xml files (for feeding
  into OgreImporter) and materials in Clockwork .xml format.
