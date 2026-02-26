versionhpp = configuration_data()

versionhpp.set('VERSION_MAJOR', version_major)
versionhpp.set('VERSION_MINOR', version_minor)
versionhpp.set('VERSION_PATCH', version_patch)
versionhpp.set_quoted('VERSION', version)

configure_file(
    input: 'version.hpp.in',
    output: 'version.hpp',
    configuration: versionhpp,
    install: true,
    install_dir: get_option('includedir') / 'god')