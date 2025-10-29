use strict;
use warnings;

my @lines = `git log --pretty=format:'%ad|%h|%s' --date=short --no-decorate`;
my %changelog;

for my $line (@lines) {
    chomp $line;
    my ($date, $hash, $message) = split /\|/, $line, 3;
    
    my ($type, $scope, $msg);
    if ($message =~ /^(\w+)(?:\(([^)]+)\))?: (.+)$/) {
        $type = $1;
        $scope = $2 // '';
        $msg = $3;
    } else {
        $type = 'other';
        $scope = '';
        $msg = $message;
    }
    
    push @{$changelog{$date}{$type}}, {
        scope => $scope,
        message => $msg,
        hash => $hash
    };
}

my @type_order = qw(feat fix docs style refactor perf test build ci chore revert other);
my %type_priority = map { $type_order[$_] => $_ } 0..$#type_order;

for my $date (sort { $b cmp $a } keys %changelog) {

    print "## ${date}\n\n";
    
    my @types = sort { 
        ($type_priority{$a} // 999) <=> ($type_priority{$b} // 999) 
    } keys %{$changelog{$date}};
    
    for my $type (@types) {
        my $heading = ucfirst($type);
        print "### $heading\n\n";
        
        for my $commit (@{$changelog{$date}{$type}}) {
            my $scope_part = $commit->{scope} ? "($commit->{scope})" : '';
            print "- **${type}${scope_part}**: " . $commit->{message} . " [" . $commit->{hash} . "]\n";
        }
        print "\n";
    }
}
