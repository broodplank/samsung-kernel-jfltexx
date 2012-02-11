#!/bin/bash
git rev-list --reverse f573aa38ae3e761413af3c01a1a58505f06790ff^..c019ca6097ebb272f0fe586df50a5639abc13704 | while read rev  
do 
git cherry-pick $rev || break
done
