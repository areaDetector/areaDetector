#!/usr/bin/env perl

# Converts HTML table to Sphinx flat-table
# Syntax:
# HMTLTableToFlatTable file
# This reads from "file.html" and writes to "file.rst", so don't give a file extension on the file name.
# It should be run on a temporary file containing only the HTML table cut from the original RST file.
# Do NOT run it on the original RST file.
# This is only designed to work on the HTML tables in the areaDetector documentation files that were nicely
# formatted using Visual Studio.  It is not very tolerant of extra white space in the input file.


@files=@ARGV;

$| = 1;
foreach( @files ) {
    open(INPUT, "<$_.html");
    open(OUTPUT, ">$_.rst");
    my $text = join "", <INPUT>;
    $text =~ s[^\s*\<tr\>\n\s*\<th\>\n\s*][  * - ]gm;
    $text =~ s[^\s*\<th\>\n\s*][    - ]gm;
    $text =~ s[^\s*\<tr\>\n\s*\<td\>\n\s*][  * - ]gm;
    $text =~ s[^\s*\<tr\>\n\s*\<td.+\n\s*][  * -\n    -\n    - ]gm;
    $text =~ s[^\s*\<td\>\n\s*][    - ]gm;
    $text =~ s[^\s*\</tr\>\n][]gm;
    $text =~ s[^\s*\</th\>\n][]gm;
    $text =~ s[^\s*\</td\>\n][]gm;
    $text =~ s[\s*\</th\>][]gm;
    $text =~ s[\s*\</td\>][]gm;
    $text =~ s[^\s*\<tbody\>\n][]gm;
    $text =~ s[^\s*\</tbody\>\n][]gm;
    $text =~ s[^\s*\</table\>\n][]gm;
    $text =~ s[\<br /\>\n\s*][, ]gm;
    $text =~ s[\<li\>][]gm;
    $text =~ s[\</li\>][ |br|]gm;
    $text =~ s[\<ul\>\n\s*][ |br|]gm;
    $text =~ s[^\s*\</ul\>\n][]gm;
    $text =~ s[^\s{5,}][      ]gm;
    $text =~ s[<code>][`]gm;
    $text =~ s[</code>][`]gm;
    $text =~ s[<b>][**]gm;
    $text =~ s[</b>][**]gm;
    my $header = ".. cssclass:: table-bordered table-striped table-hover\n";
    $header .= ".. flat-table::\n";
    $header .= "  :header-rows: 2\n";
    print OUTPUT $header;
    print OUTPUT $text;
    close INPUT;
    close OUTPUT;
}
