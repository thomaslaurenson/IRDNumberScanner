# IRDNumberScanner

A bulk_extractor scanner plug-in to detect and validate Inland Revenue (IRD) Numbers.

## Install Instructions

This plug-in has been tested with bulk_extractor version 1.5.5 and the development code hosted on GitHub (dated: 2018/05/15, commit: ecb627d7b60d5a34b51639a8deaffc4db59fda27). The following installation instructions outline how to install the IRDNumberScanner on the Git development version, using Ubuntu 18.04 LTS.

Make sure git in installed:

`sudo apt install git`

Clone the official bulk\_extractor repository:

`git clone --recursive https://github.com/simsong/bulk_extractor.git` 

Change to the bulk_extractor directory:

`cd bulk_extractor`

Execute the provided script to configure Ubuntu system, which installs various system dependencies:

`bash etc/CONFIGURE_UBUNTU18.bash`

Run the bootstrap script:

`chmod u+x bootstrap.sh`

`./bootstrap.sh`

Clone this (IRDNumberScanner) repository:

`git clone https://github.com/thomaslaurenson/IRDNumberScanner.git`

Copy the two required files (`scan_ird.flex` and `Makefile.am`) to the plugins directory for bulk\_extractor. These instructions that both bulk\_extractor and IRDNumberScanner repositories are in your home directory:

`cp ~/IRDNumberScanner/scan_ird.flex ~/bulk_extractor/plugins/scan_ird.flex`

`cp ~/IRDNumberScanner/Makefile.am ~/bulk_extractor/plugins/Makefile.am`

Just to be clear: The `scan_ird.flex` file is the code implemented to perform IRD number scanning that is implemented as a plugin to bulk\_extractor. While the `Makefile.am` is a modified version of the original bulk\_extractor Makefile, used to compile the plugin source code. The only modifications to the `Makefile.am` file is the inclusion of the scan\_ird plugin, and disabling of the scan\_flexdemo plugin (as it was not operating correctly at time of the last development on this project).

Make sure you are in the bulk\_extractor directory:

`cd ~/bulk_extractor`

Run the configure script:

`./configure`

Now, compile (or make) the entire bulk\_extractor porject:

`make`

Finally, the plugins also require compilation:

`cd ~/bulk_extractor/plugins/`

`make plugins`

You can either install bulk\_extractor system wide using the following command:

`sudo make install`

`bulk_extractor`

Or, simply use the created binaries in the project:

`cd ~/bulk_extractor`

`./src/bulk_extractor`

7) Run bulk_extractor from compiled source code:
 
`cd src`

`./bulk_extractor -P ../plugins -h`
