#!/usr/bin/env perl

use strict;
use warnings;

my ($debug) = @ARGV;

my $ON_ARM = `grep ARM /proc/cpuinfo | head -n 1`;
my $FLAGS = "";

if ($ON_ARM) {
    print "Running on ARM\n";
    $FLAGS .= "-mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -funsafe-math-optimizations";
} else {
    print "Not running on ARM\n";
}

if ($debug) {
    $FLAGS .= " -DDEBUG";
}

`cd build; CXXFLAGS=\"$FLAGS\" cmake ../src`;
