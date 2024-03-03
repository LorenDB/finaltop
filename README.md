# finaltop

finaltop is a toy system monitor application written using [FINAL CUT](https://github.com/gansm/finalcut).
You can read more about the development of finaltop [here](https://lorendb.dev/posts/cpp-spotlight-final-cut-gui).

## Building

You'll need to install the FINAL CUT library on your system (`libfinal-devel` on openSUSE). Then it's
the normal CMake routine:

```
git clone https://github.com/LorenDB/finaltop.git
cd finaltop
mkdir build; cd build
cmake ..
cmake --build .
```
