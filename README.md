# IRDNumberScanner

A bulk\_extractor scanner plug-in to detect and validate New Zealand Inland Revenue (IR) numbers. They are commonly referred to as IRD numbers or IR numbers. This number is similar to the US-based Social Security Number (SSN). IRD numbers are used by both businesses and individuals for all tax, entitlements and personal details held by the Inland Revenue, a New Zealand government department which administers government revenue and various social support programmes.

The purpose of this project is to provide a plug-in to the well-known bulk\_extractor tool - a computer forensics tool used to scan electronic data and extract artifacts of interest. In this case, the artifacts of interest are IRD numbers. Bulk\_extractor is unique in the fact that it can process any type of data input, such as forensic disk images, folders of files and network traffic captures.

## Quickstart: Ubutnu 18.04

Download bulk extractor:

`git clone --recursive https://github.com/simsong/bulk_extractor.git` 

Download IRDNumberScanner and copy files to plugins directory:

`git clone https://github.com/thomaslaurenson/IRDNumberScanner.git`

`cp ~/IRDNumberScanner/scan_ird.flex ~/bulk_extractor/plugins/scan_ird.flex`

`cp ~/IRDNumberScanner/Makefile.am ~/bulk_extractor/plugins/Makefile.am`

Compile bulk\_extractor:

`cd ~/bulk_extractor`

`bash etc/CONFIGURE_UBUNTU18.bash`

`./bootstrap.sh`

`./configure`

`make`

`cd plugins`

`make plugins`

## Associated Project Material

This repository hosts the code produced as a result of the research performed by Henry Gee for his Master's thesis research conducted at the University of Otago. In addition, I have continued development of the project adding additional tools and documentation so that the project remains relevant and useful to computer forensic practitioners.

You can access Henry Gee Master's Thesis from the [OUR Archive](http://hdl.handle.net/10523/6261) hosted by the University of Otago. In addition, you can access the resultant academic publication of the paper submitted to the 2015 Australian Digital Forensics Conference, available online from the [Edith Cowen University Research Online](http://ro.ecu.edu.au/cgi/viewcontent.cgi?article=1149&context=adf) repository. Finally, the associated conference presentation entitled [Improving the Detection and Validation of Inland Revenue Numbers](http://thomaslaurenson.com/files/SECAU2015_IRD.pdf) is available on my personal website.

## Project Structure

This project comes with a collection of useful programs and files. Below is a brief summary of the contents of the project:

- `scan_ird.flex`: The source code for the scan_ird plug-in. This file can be copied to the bulk\_extractor source code plugins directory and included in the bulk\_extractor compilation process
- `Makefile.am`: A modified bulk\_extractor plug-ins Makefile. This file can be copied to the bulk\_extractor source code plugins directory and includes the changes to include compilation of the `scan_ird.flex` file, so that it is available for use in the bulk\_extractor tool
- `validate_numbers.cpp`: A small C++ program which uses the base code provided in the `scan_ird.flex` plug-in. The program is suitable for testing and development, as well as performing direct IRD number validation without running the bulk\_extractor tool.
- `blacklist.txt`: The bulk\_extractor tool has the functionality to include a blacklist of known, irrelevant results with any scanner. This blacklist has been compiled by running the scan\_ird tool against various default operating system installs. If a specific IRD number exists in a default operating system install it can be concluded that it is not unique or relevant. It is important to note that the blacklist uses contextual analysis, it is not just the IRD number that is blacklisted, it is the IRD number and the immediate data before and after the number as well. This makes the blacklist highly accurate at automating the correct removal of irrelevant results. The bulk\_extractor project has more extensive documentation on this functionality.

## IRD Number Format and Validation Method

New Zealand Inland Revenue (IR/IRD) numbers have a specified number format and validation method. The most current documentation (2018) from the Inland Revenue department is [available online as a PDF](http://www.ird.govt.nz/resources/e/c/ec5956fb-7427-4aba-97f6-74f54e0623c0/payroll_payday_filing_spec_2019_+v1.0.pdf). Specifically, see pages 33 to 35 of the provided link. Below is a summary of the IRD number structure and validation process.

An IRD number is an eight or nine digit number consisting of the following components:

- A seven (7) or eight (8) digit base number
- A trailing check digit

### IRD Number Format

New Zealand IRD numbers may be stored in a variety of different structures or formats. Although an IRD number is an 8 or 9 digit number, it is commonly stored with spaces or dash delimiters. However, there is no standardised storage method. An example of the storage format is provided below:

- IRD Number: `49091850`
- Dash delimiters: `49-091-850`
- Space delimiters: `49 091 850`

A full table of potential storage formats is provided in the table below. The scan\_ird plug-in search for all potential IRD numbers as documented by the table below:

**IRD Number Description** | **IRD Number Structure** | **Example**
-----|-----|-----
8 digits | NNNNNNNN | 49091850
8 digits with space delimiter | NN NNN NNN | 49 091 850
8 digits with dash delimiter | NN-NNN-NNN | 49-091-850
9 digits | NNNNNNNNN | 136410133
9 digits with space delimiter | NNN NNN NNN | 136 410 133
9 digits with dash delimiter | NNN-NNN-NNN | 136-410-133

## Project Installation Instructions

This plug-in has been tested with bulk\_extractor version 1.5.5 and the development code hosted on GitHub (dated: 2018/05/15, commit: ecb627d7b60d5a34b51639a8deaffc4db59fda27). The following installation instructions outline how to install the IRDNumberScanner on the Git development version, using Ubuntu 18.04 LTS.

Make sure git is installed:

`sudo apt install git`

This documentation expects you are in the home directory of your user account:

`cd ~`

Clone the official bulk\_extractor repository:

`git clone --recursive https://github.com/simsong/bulk_extractor.git` 

Change to the bulk\_extractor directory:

`cd ~/bulk_extractor`

Execute the provided script to configure Ubuntu system, which installs various system dependencies:

`bash etc/CONFIGURE_UBUNTU18.bash`

Clone this (IRDNumberScanner) repository:

`cd ~`

`git clone https://github.com/thomaslaurenson/IRDNumberScanner.git`

Copy the two required files (`scan_ird.flex` and `Makefile.am`) to the plugins directory for bulk\_extractor. These instructions expect that both bulk\_extractor and IRDNumberScanner repositories have been cloned directly into your home directory:

`cp ~/IRDNumberScanner/scan_ird.flex ~/bulk_extractor/plugins/scan_ird.flex`

`cp ~/IRDNumberScanner/Makefile.am ~/bulk_extractor/plugins/Makefile.am`

Just to be clear: The `scan_ird.flex` file is the code implemented to perform IRD number scanning that is implemented as a plugin to bulk\_extractor. While the `Makefile.am` is a modified version of the original bulk\_extractor Makefile, used to compile the plugin source code. The only modifications to the `Makefile.am` file is the inclusion of the scan\_ird plugin, and disabling of the scan\_flexdemo plugin (as it was not operating correctly at time of the last development on this project).

Make sure you are in the bulk\_extractor directory:

`cd ~/bulk_extractor`

Run the bootstrap script:

`chmod u+x bootstrap.sh`

`./bootstrap.sh`

Run the configure script:

`./configure`

Now, compile (or make) the entire bulk\_extractor porject:

`make`

Finally, the plugins also require compilation:

`cd ~/bulk_extractor/plugins/`

`make plugins`

You can either install bulk\_extractor system wide using the following command:

`sudo make install`

Or, simply use the created binaries in the project:

`cd ~/bulk_extractor`

Then run the bulk\_extractor binary:

`./src/bulk_extractor`

You can also specifically include the plug-ins directory using the `-P` command line argument: 

`./src/bulk_extractor -P plugins`

You can verify that the `scan_ird` plug-in has been successfully compiled and is available using the following command:

`./src/bulk_extractor -P plugins -h | grep ird`

Finally, below is an example command of how to run the IRD scanner:

`./src/bulk_extractor -P plugins -E ird -o ~/test-run -R ~/sample-ird-numbers.txt`

This command specifies that only the IRD scanner should be run (`-E ird`), that the directory for output is saved in a folder called `test-run` in the home directory (`-o ~/test-run`), and that the IRD scanner is executed against a file named `sample-ird-numbers.txt` (`-R ~/sample-ird-numbers.txt`).

## TODO

- ~~Finish documentation for IRD number validation section of README.md~~
- ~~Implement Inland Revenue provided examples for testing in validate\_numbers program~~
- ~~Implement valid IR number generation in validate\_numbers program~~
- Re-implement histogram functionality when resolved