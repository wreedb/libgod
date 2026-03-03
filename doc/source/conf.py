project = "libgod"
copyright = "2026, Will Reed"
author = "Will Reed"
release = "0.1.0"

extensions = ["breathe", "sphinx.ext.autodoc"]
breathe_projects = {"libgod": "doxygen/xml"}
breathe_default_project = "libgod"

exclude_patterns = []
html_theme = "pydata_sphinx_theme"
html_static_path = ["_static"]
templates_path = ["_templates"]

# html_css_files = ["custom.css"]