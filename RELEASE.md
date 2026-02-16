# RELEASE 流程（半自动：Agent 准备 + 人类确认）

## 目标
把 develop 中已验证的变更，发布到 main，并打 SemVer tag（vMAJOR.MINOR.PATCH）。

---

## 0. 发版前硬性条件（Gate）
- [ ] `develop` 最新提交已通过 CI（绿色）
- [ ] 关键冒烟用例已跑过（由 agent 输出清单与结果）
- [ ] 本次发版包含的 GitHub Issues 列表已整理（例如：#123 #87 ...）
- [ ] 回滚方案明确（至少能指出"回滚哪个 merge commit 或哪些关键 commit"）

---

## 1. Agent 负责：发版准备（在 develop 上）

1) 拉取最新
```bash
git checkout develop
git pull origin develop
```

2) 选定版本号（SemVer）
- PATCH：仅 bugfix
- MINOR：新增功能（向后兼容）
- MAJOR：破坏性变更（不兼容）

3) 更新版本号（按项目约定）
- platformio.ini 或其他版本文件

4) 更新 CHANGELOG（如果存在）
- 按 "Added / Changed / Fixed / Breaking" 分类写清楚

5) 生成 Release Notes 草稿（必须包含）
- 版本：vX.Y.Z
- 变更摘要（按 feat/fix/refactor…）
- 关联 Issues：#123 #87 ...
- 风险点与回滚指引（指向 commit sha 或 merge commit sha）
- 验证命令与结果（例如 `pio run`）

6) 提交发版准备 commit（Conventional Commits + 中文）示例：
```bash
git add -A
git commit -m "chore(release): 准备 vX.Y.Z

Refs: #123"
git push origin develop
```

**注意**：这里只是"准备"，还没有动 main。

---

## 2. 人类确认点（必须停下来等确认）

- [ ] 我已阅读 Release Notes 草稿
- [ ] 我同意版本号 vX.Y.Z
- [ ] 我同意发布时间点

确认后才进入下一步。

---

## 3. 发版执行（merge develop -> main + tag）

```bash
git checkout main
git pull origin main

git merge --no-ff develop -m "release: vX.Y.Z"
git tag -a vX.Y.Z -m "Release vX.Y.Z"
git push origin main --tags
```

---

## 4. GitHub Release

- 创建 GitHub Release，标题：vX.Y.Z
- 粘贴 Release Notes（用 agent 草稿为基础）

---

## 5. 发版后收尾

- 关闭本次已交付的 GitHub Issues（在 issue 里留言：已在 vX.Y.Z 发布）
- 若发现严重问题：只允许用 git revert 回滚（禁止 reset + force push）

---

## 6. 回滚手册（只用 revert）

回滚一次发布（推荐：revert main 上的 release merge commit）

```bash
git checkout main
git pull origin main
git log --oneline --decorate --graph -20
# 找到 "release: vX.Y.Z" 对应的 merge commit sha

git revert -m 1 <merge_commit_sha>
git push origin main
# 然后打一个新 PATCH 版本 tag（例如 vX.Y.(Z+1)）
```
