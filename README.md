# IRDNumberScanner
A bulk_extractor scanner plug-in to detect and validate Inland Revenue (IRD) Numbers

# Install
This plug-in has been tested with bulk_extractor version 1.4

1) Copy the following files to bulk_extractor-1.4.0/plugins
        scan_ird.flex
        Makefile.am

2) Make sure you are in the root application directory (bulk_extractor-1.4.0)

3) Run autoconf again to create an updated Makefile.in from original Makefile.ac:
        autoreconf

4) Configure the build environment (this will generate a new plugins/Makefile that is fixed):
        ./configure

5) Make the entire project:
        make
       
6) Make the plugins:
        (not sure why this step is required... tried to find how to do this automatically from the root Makefile.am, but not sure!)
        cd plugins
        make plugins
        cd ..
       
7) Run bulk_extractor from compiled source code:
        cd src
        ./bulk_extractor -P ../plugins -h
