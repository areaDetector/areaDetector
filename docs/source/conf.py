#
# asyn documentation

import sys, os

project = 'areaDetector'
html_title = html_short_title = 'areaDetector Support'

authors = 'Mark Rivers'
copyright = '2023, Mark Rivers'

extensions = ['sphinx.ext.autodoc',
              'sphinx.ext.mathjax',
              'sphinx.ext.extlinks',
              'sphinx.ext.napoleon',
              'm2r2',
              'linuxdoc.rstFlatTable'
]

todo_include_todos = True

templates_path = ['_templates']
source_suffix = ['.rst', '.md']
source_encoding = 'utf-8'
master_doc = 'index'
today_fmt = '%Y-%B-%d'

exclude_trees = ['_build']

add_function_parentheses = True
add_module_names = False
pygments_style = 'sphinx'

# html themes: 'default', 'sphinxdoc',  'alabaster', 'agogo', 'nature', 'pyramid'
#html_theme = 'pyramid'
html_theme = 'sphinx_rtd_theme'

html_static_path = ['_static']
html_style = 'css/theme_overrides.css'
html_last_updated_fmt = '%Y-%B-%d'
html_show_sourcelink = True
htmlhelp_basename = 'areaDetectordoc'
