# -*- mode: nix; -*- vim:ft=nix
{
    user = {
        
        name = "Will";
        age = 26;
        married = false;

        friends = [
            
            {
                name = "Ross";
                age = 29;
                married = true;
                favorite-numbers = [ 1 2 -3.14 false null true "Hello!" 69 ];
                qualities = {
                    emotional = [ "patient" 1 "nice" null ];
                };
            }

        ];
        
    };
}
