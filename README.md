WoT map viewer
--------------

Show World of Tanks minimap in external program.

It is needed to install mod to your wot client that communicates with the map
viewer program. The viewer can then be displayed in another monitor for
example.

Installing mod
--------------

To install the mod, you must first install XVM.

Then create folder `WOT_DIR/res_mods/WOT_VERSION/scripts/client/gui/mods/mapserver`

Compile the `mod/*py` files (`python2.7 -m compileall *.py` should do the trick).
and copy the .pyc files to mapserver folder

Building/Running viewer
-----------------------

On Windows, there should be prebuild binaries available soon.

On Linux, first install qt-sdk (from your distribution software repositories).
Then issue following commands

    qmake
    make

Run the viewer-executable in the folder where img and qml -folders are located.
You must have WoT running with the mod installed and you must be in battle to
really view the minimap.

Legal
-----

GPL v3 license and so on.
