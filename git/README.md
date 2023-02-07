# Git

## Delete all merged branches

```sh
git branch --merged main | grep -v "\* main" | xargs -n 1 git branch -d
```
