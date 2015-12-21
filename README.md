# IRDNumberScanner

A bulk_extractor scanner plug-in to detect and validate Inland Revenue (IRD) Numbers.

## Install Instructions

This plug-in has been tested with bulk_extractor version 1.4.0. You can download bulk_extractor from the digital corpora website:

`wget http://digitalcorpora.org/downloads/bulk_extractor/old/bulk_extractor-1.4.0.tar.gz` 

1) Copy the following files to bulk\_extractor-1.4.0/plugins. The scan\_ird.flex contains the IRDNumberScanner source code, the Makefile.am is an updated Makefile to resolve some compilation issues with the demo scanner plug-ins bundled with bulk_extractor. 

`scan_ird.flex`

`Makefile.am`

2) Make sure you are in the root application directory (bulk_extractor-1.4.0/)

3) Run autoconf again to create an updated Makefile.in from original Makefile.ac:

`autoreconf`

4) Configure the build environment (this will generate a new plugins/Makefile that is fixed):

`./configure`

5) Make the entire project:

`make`
       
6) Make the plugins:

`cd plugins`

`make plugins`

`cd ..`
       
7) Run bulk_extractor from compiled source code:
 
`cd src`

`./bulk_extractor -P ../plugins -h`
