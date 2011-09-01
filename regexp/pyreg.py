# Script usage: python script.py <filename.html>
import sys
import re

"""Return a string list text grabbed from HTML."""
def html_text_regex(text):
    # @type text str
    pattern = '>([^<\t\r\n]+)<'
    strings = re.findall(pattern,text)
    print_strings(strings)

"""Print strings as a list."""
def print_list(strings):
    # @type strings list
    print strings

"""Print every string as a new line."""
def print_strings(strings):
    # @type strings list
    for string in strings: print(string)

"""Read contetnts from file, parse HTML and print results."""
def main(filename):
    try:
        f = open(filename, "r")
        filestring = f.read()
        html_text_regex(filestring)
        f.close()
    except IOError as (errno, strerror):
        print "I/O error({0}): {1}".format(errno, strerror)
    except:
        print "Unexpected error:", sys.exc_info()[0]

###################################
if __name__ == "__main__":
    try:
        html_file = sys.argv[1]
    except:
        print "Usage: python pyreg.py <filename.html>"
        sys.exit(1)
    main(html_file)
