#!/usr/bin/env bash

set -u

if [ $# -lt 2 ]; then
  echo "usage: $0 <previous-tag> <current-tag>"
  exit 1
fi

github_url="https://github.com/areaDetector"

# Find all tags after a given commit for a module
get_module_releases() {
  module=$1
  commit=$2

  # Take commit right after the given one
  first_commit=$(git -C $module log --format=%h --reverse $commit..HEAD | head -n 1)

  tags="$(git -C $module tag --contains $first_commit)"

  echo $tags
}

# Print release information of each module based on the provided commit
print_module_releases() {
  module=$1
  commit=$2

  read -ra tags <<< "$(get_module_releases $module $commit)"

  if [ ${#tags[@]} -gt 0 ]; then
    echo "* [$module ${tags[@]}]($github_url/$module/blob/master/RELEASE.md#${tags[-1],,})"
  else
    echo "* $module"
  fi
}

previous_tag=$1
current_tag=$2

changed_files=$(git diff --name-only $previous_tag..$current_tag)

printf "### $current_tag\n\n"

for file in $changed_files; do
  # Check if changed file is a submodule
  tree_info=$(echo "$(git ls-tree $previous_tag)" | grep -oE ".*commit.*$file")

  if [ -n "$tree_info" ]; then
    module=$(echo $tree_info | cut -d' ' -f 4)
    commit=$(echo $tree_info | cut -d' ' -f 3)

    print_module_releases $module $commit
  fi
done
