import collections

if __name__=='__main__':
    import argparse
    parser = argparse.ArgumentParser(description='''
search_annotated.py parses an annotated IRD feature file and searches the feature context using a selection of keywords.''',
formatter_class = argparse.RawTextHelpFormatter)
    parser.add_argument('annotated_ird',
                        help = 'annotated output file')
    args = parser.parse_args()

    fi = args.annotated_ird

    entries = list()
    Entry = collections.namedtuple('Entry', 'location feature context filename md5')

    print("\n>>> Process annotated IRD feature file...")
    print(">>> Annotated file: %s\n" % fi)

    with open(fi) as f:
        for line in f:
            if line.startswith("#"):
                continue
            line = line.strip()
            line = line.split("\t")
            if len(line) == 3:
                entry = Entry(location=line[0], feature=line[1], context=line[2], filename=None, md5=None)
            elif len(line) == 4:
                entry = Entry(location=line[0], feature=line[1], context=line[2], filename=line[3], md5=None)
            elif len(line) == 5:
                entry = Entry(location=line[0], feature=line[1], context=line[2], filename=line[3], md5=line[4])
            entries.append(entry)

    for e in entries:
        if "GST" in e.context:
            print(">>> Potential IRD number with 'GST' keyword:")
            print("  > IRD Number: %s" % e.feature)
            print("  > Location:   %s" % e.location)
            print("  > Context:    %s" % e.context)                        
            print("  > File name:  %s" % e.filename)
            print("  > File MD5:   %s\n" % e.md5)
        if "IRD" in e.context:
            print(">>> Potential IRD number with 'IRD' keyword:")
            print("  > IRD Number: %s" % e.feature)
            print("  > Location:   %s" % e.location)
            print("  > Context:    %s" % e.context)                        
            print("  > File name:  %s" % e.filename)
            print("  > File MD5:   %s\n" % e.md5)
        if "Inland" in e.context:
            print(">>> Potential IRD number with 'Inland' keyword:")
            print("  > IRD Number: %s" % e.feature)
            print("  > Location:   %s" % e.location)
            print("  > Context:    %s" % e.context)                        
            print("  > File name:  %s" % e.filename)
            print("  > File MD5:   %s\n" % e.md5)
        if "Invoice" in e.context:
            print(">>> Potential IRD number with 'Invoice' keyword:")
            print("  > IRD Number: %s" % e.feature)
            print("  > Location:   %s" % e.location)
            print("  > Context:    %s" % e.context)                        
            print("  > File name:  %s" % e.filename)
            print("  > File MD5:   %s\n" % e.md5)
        if "Transaction" in e.context:
            print(">>> Potential IRD number with 'Transaction' keyword:")
            print("  > IRD Number: %s" % e.feature)
            print("  > Location:   %s" % e.location)
            print("  > Context:    %s" % e.context)                        
            print("  > File name:  %s" % e.filename)
            print("  > File MD5:   %s\n" % e.md5)
