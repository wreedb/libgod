# Contributing
If you'd like to contribute to **Libgod**, you must agree that any and all 
merged contributions will be licensed under the **GNU Lesser General Public License** 
version 3 (or later). For information about this license, see the [license text](LICENSE.md) 
included in the project source, or the corresponding page on [gnu.org](https://gnu.org/licenses/lgpl-3.0.html)

## Usage of AI
The usage of AI generated code is not allowed in this project for a number of  
reason, though this should not really need to be explained. Though the  
principle reason for this stance is legal ambiguity, as large language models  
are trained on data irrespective of whether or not the consumer of the **stolen**  
code accepts or intends to comply with the licensing terms of the project it was  
usurped from.

## Use of third-party code
If your contributions make use of (or vendor) any third-party code, you must  
ensure that the code in question is compatible for use in a project using the  
**LGPLv3**. You must also make sure to enact any requirements made by the  
third-party license; such as including its license alongside Libgod. This  
project takes licensing extremely serious, and any contributions must reflect  
this.

## Keeping track
**Libgod** adheres to the [REUSE](https://reuse.software) specification; 
Changes made should be annotated in-file or in `REUSE.toml` in the 
projects' root directory. See the [specification](https://reuse.software/spec/) and [FAQ](https://reuse.software/faq/) for more information. 
All potential contributors should use the [REUSE tool](https://github.com/fsfe/reuse-tool) to run 
`reuse lint` before making submissions.

## Why are these policies so strict?
Some developers wish to free themselves of interaction with copyright and 
licensing matters, however the idea of such freedom is not obtained through 
a hands-off approach. Freedom is not granting all to do as they wish; 
it is granting all to do as they wish without encroaching on others' freedom 
or having their freedom encroached upon.

## Exceptions
There are absolutely no exceptions.

## Hygiene
This project follows the [Conventional Commits](https://conventionalcommits.org/en/v1.0.0/#summary) specification, and therefore  
contributions must also. For ease of use, the author recommends using the  
[convco](https://convco.github.io) tool for commits, as it helps streamline  
the process. Following these rules allows the project to automatically generate  
changelogs, compatible with the [Keep a Changelog](https://keepachangelog.com) specification.

Also strictly followed is the [Semver 2.0.0](https://semver.org) specification, though it is  
unlikely contributors will need to concern themselves with versioning.

# Code Style
We make generous use of namespaces, not just for organization, but it also has  
the effect of reducing the need for the use of many functions or classes  
needing a multi-word name; that being said, snake case is generally preferred  
over camel-case.

Capital letters are the enemy: the less you use them, the less any of us  
have to reach our pinkies to the shift key. The only time they are tolerated  
is in the event that it would cause naming collisions, which is pretty rare.  
As of writing (2026-02-26), we only have one variable/class/struct/function  
with a capital letter in its' name.  

Never use formatters like `clang-format` on the code in the repository unless  
it was written by you. Write with intention and care, and formatters are  
never needed.  