#!/bin/bash

RED='\033[0;31;1m'
GREEN='\033[0;32;1m'
BLUE='\033[0;34;1m'
YELLOW='\033[1;33;1m'
NORMAL='\033[0m'

echo "${BLUE}Configuring git hooks...${NORMAL}"

mv .git/hooks/commit-msg.sample .git/hooks/commit-msg && cat ./hooks/commit-msg > .git/hooks/commit-msg

if [ $? -eq 1 ]
then
  echo "${RED}Failed. Could not copy commit-msg hooks from ./hooks/commit-msg to .git/hooks/commit-msg.\n${NORMAL}"
  exit 1
fi

echo "${GREEN}Configured commit-msg hook successfully.\n${NORMAL}"

mv .git/hooks/pre-commit.sample .git/hooks/pre-commit && cat ./hooks/pre-commit > .git/hooks/pre-commit

if [ $? -eq 1 ]
then
  echo "${RED}Failed. Could not copy pre-commit hooks from ./hooks/pre-commit to .git/hooks/pre-commit.\n${NORMAL}"
  exit 1
fi

echo "${GREEN}Configured pre-commit hook successfully.\n${NORMAL}"

echo "${GREEN}Done :)\n${NORMAL}"