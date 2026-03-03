Quick Start
===========

Let's get you up and running with a simple example: parsing a file
and printing it out as JSON!

We can write a sample input file to use:

.. code-block:: nix

    # file.god
    {
        name = "Will";
        favorite-numbers = [ 7 222 ];
    }


And then we can write a program to parse it and print it out in
just a handful of lines:
   
.. code-block:: cpp
   
    #include <god.hpp>
   
    auto main(int argc, const char **argv) -> int {
        god::document doc;
        doc.load("file.god");
        std::cout << doc << '\n';
    }
    
We should see output similar to:

.. code-block:: bash
    
    $ ./sample
    {"name": "Will", "favorite-numbers": [7, 222]}
    

A fantastic first step, but what if we just want a specific value? for example,
the second element of the ``favorite-numbers`` list?

.. code-block:: cpp

    god::document doc;
    doc.load("file.god");
    
    auto result = doc.query<god::list>("favorite-numbers");
    
    // If we arent able to find our query, we can
    // 'die', displaying a descriptive error message.
    if (not result) result.error().die();
    
    std::cout << result.value().at(1).as<std::int64_t>() << '\n';
    

From this, we should recieve just the second number in the list:

.. code-block:: bash
    
    $ ./sample
    222
    

Let's bring it up a notch, our new input file:

.. code-block:: nix
    
    {
        users = {
            will = {
                login = "wbr";
                age = 27;
                root-privilege = true;
            };
        };
    }
    
Now lets put together our own ``user`` struct to hold the data about Will.

.. code-block:: cpp

    #include <god.hpp>
    #include <print>
    
    struct user {
        std::string login;
        int age;
        bool root_privilege;
    };
    
    auto main(int argc, const char **argv) -> int {
        
        god::document doc;
        doc.load("file.god");
        
        // First, lets get a reference to the 'will' map:
        
        auto result = doc.query<god::map>("users.will");
        if (not result) result.error().die();
        
        auto name = result.value()["login"];
        if (not name) name.error().die();
        
        auto age = result.value()["age"];
        if (not age) age.error().die();
        
        auto root_priv = result.value()["root-privilege"];
        if (not root_priv) root_priv.error().die();
        
        user will = {
            .login = name.value()->val.as<std::string>(),
            .age = static_cast<int>(age.value()->val.as<std::int64_t>()),
            .root_privilege = root_priv.value()->val.as<bool>()
        };
        
        std::println(
            "hello user '{}', you are {} years old and {} root privilege!",
            will.login,
            will.age,
            will.root_privilege ? "have" : "do NOT have"
        );
        return 0;
    }
    
Now, upon running this, we should see:

.. code-block:: bash
    
    $ ./sample
    Hello user 'wbr', you are 27 years old and have root privilege!
    

And now, perhaps we could try synthesizing a document out of thin air (bits)!

.. code-block:: cpp

    god::document doc;
    god::list my_favorite_numbers_value({
        god::value(867),
        god::value(5309)
    });
    
    god::field my_field("my-favorite-numbers", my_favorite_numbers_val);
    
    doc.add(my_field);
    
    std::cout << god::pretty::document(doc) << '\n';
    

This should provide us the output:

.. code-block:: bash

    $ ./sample
    {
        my-favorite-numbers = [
            867
            5309
        ];
    
    }