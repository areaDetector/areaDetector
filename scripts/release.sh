#!/usr/bin/env bash

set -u

if [ $# -lt 2 ]; then
  echo "usage: $0 <previous-tag> <current-tag>"
  exit 1
fi

github_url="https://github.com/areaDetector"


# Find all tags in a given commit range (start_commit, end_commit] for a module
get_module_releases() {
  module=$1
  start_commit=$2
  end_commit=$3

  # Take commit right after the first one
  first_commit=$(git -C $module log --format=%h --reverse $start_commit..$end_commit | head -n 1)

  tags="$(git -C $module tag --contains $first_commit --no-contains $end_commit)"

  # Include `end_commit` to the range
  tags+=" $(git -C $module tag --points-at $end_commit)"

  echo $tags
}

# Print release information of each module based on the provided commit range
# and releases
print_module_releases() {
  module=$1
  start_commit=$2
  end_commit=$3
  releases="${@:4}"

  read -ra tags <<< "$releases"

  release_notes_url=$github_url/$module/blob/master/RELEASE.md
  details_url=$github_url/$module

  if [ "$start_commit" != "none" ]; then
      details_url=$details_url/compare/$start_commit..$end_commit
  fi

  if [ ${#tags[@]} -gt 0 ]; then
    echo "* [$module]($details_url) [${tags[@]}]($release_notes_url#${tags[-1],,})"
  else
    echo "* [$module]($details_url)"
  fi
}

previous_tag=$1
current_tag=$2

changed_files=$(git diff --name-only $previous_tag..$current_tag)

tagged=()
untagged=()
added=()

for file in $changed_files; do
  previous_tree=$(echo "$(git ls-tree $previous_tag)" | grep -oE ".*commit.*$file")
  current_tree=$(echo "$(git ls-tree $current_tag)" | grep -oE ".*commit.*$file")

  if [ -z "$previous_tree" -a -z "$current_tree" ]; then
    # Changed file is not a submodule
    continue
  fi

  start_commit=$(echo $previous_tree | cut -d' ' -f 3)
  end_commit=$(echo $current_tree | cut -d' ' -f 3)
  module=$(echo $current_tree | cut -d' ' -f 4)

  if [ -z "$previous_tree" -a -n "$current_tree" ]; then
    start_commit=$(git -C $module rev-list --max-parents=0 --reverse HEAD | head -n 1)
    releases=$(get_module_releases $module $start_commit $end_commit)

    added+=("$module none $end_commit $releases")
  elif [ -n "$previous_tree" -a -n "$current_tree" ]; then
    releases=$(get_module_releases $module $start_commit $end_commit)

    if [ -n "$releases" ]; then
      tagged+=("$module $start_commit $end_commit $releases")
    else
      untagged+=("$module $start_commit $end_commit")
    fi
  fi
done

printf "### $current_tag\n\n"

if [ ${#tagged[@]} -gt 0 ]; then
  printf "#### Module releases\n\n"

  for module_info in "${tagged[@]}"; do
    print_module_releases $module_info
  done
fi

if [ ${#untagged[@]} -gt 0 ]; then
  printf "\n#### Modules with untagged updates\n\n"

  for module_info in "${untagged[@]}"; do
    print_module_releases $module_info
  done
fi

if [ ${#added[@]} -gt 0 ]; then
  printf "\n#### New modules\n\n"

  for module_info in "${added[@]}"; do
    print_module_releases $module_info
  done
fi
