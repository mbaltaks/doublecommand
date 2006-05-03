#!/usr/bin/perl

use strict;

my $numargs = @ARGV;

if ( @ARGV < 1 ) {
    die "Usage: $0 version-file-path < source > dest\n";
}

my $VersionFile = shift( @ARGV );
my $Version;

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

my $change = 0;
my $long_version = 0;

while ( <> ) {
    if (/CFBundleVersion/)
    {
    	$change = 1;
    	print;
    	next;
    }
    if (/CFBundleShortVersionString/)
    {
    	$change = 1;
    	print;
    	next;
    }
    if (/CFBundleGetInfoString/)
    {
    	$long_version = 1;
    	print;
    	next;
    }
    if ($change == 1)
    {
    	s/(<string>).+(<\/string>)/\1$Version\2/g;
    	$change = 0;
    }
    if ($long_version == 1)
    {
    	s/(<string>).+(<\/string>)/\1DoubleCommand $Version\2/g;
    	$long_version = 0;
    }
    
    print;
}
