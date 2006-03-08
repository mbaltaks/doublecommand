#!/usr/bin/perl

use strict;

my $numargs = @ARGV;

if ( @ARGV < 1 ) {
    die "Usage: $0 version-file-path < source > dest\n";
}

my $VersionFile = shift( @ARGV );
my $Version;
#my $inputFile = shift(@ARGV);

if ( @ARGV ) {
    die "Usage: $0 version-file-path < source > dest\n";
}

if ( ! open( VERSION, "< $VersionFile" ) ) {
    die "Couldn't open $VersionFile for reading: $!\n";
}

while ( <VERSION> ) {
    if ( /^#/ ) {
	next;
    }

    chomp;

    if ( !length ) {
	next;
    }

    s/^\s*//;
    s/\s*$//;

    $Version = $_;
    last;
}

if ( !defined( $Version ) ) {
    die "No version defined in $VersionFile\n";
}

#if ( ! open( in, "< $inputFile" ) ) {
#    die "Couldn't open $inputFile for reading: $!\n";
#}

#binmode(in, ":encoding(UTF-16)");
binmode(STDIN, ":encoding(UTF-16)");
binmode(STDOUT, ":encoding(UTF-16)");

while ( <> ) {
	if (/version .+,/)
	{
		s/(version ).+(,.+)/\1$Version\2/g;
	}
	else
	{
		s/(version ).+(\")/\1$Version\2/g;
	}
    print;
}
