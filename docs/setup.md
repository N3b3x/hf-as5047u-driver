---
layout: default
title: "🛠️ Setup"
description: "Prerequisites and how to obtain the AS5047U driver source"
nav_order: 1
parent: "📚 Documentation"
permalink: /docs/setup/
---

# 🛠️ Setup

This guide walks you through preparing your environment and cloning the repository.

## Prerequisites

- ✅ **C++20 compiler** (e.g. `g++` 10+)
- ✅ **Make** utility
- ✅ **Git** for cloning the sources

These packages are available on most Linux distributions. On Debian/Ubuntu you can install them with:

```bash
sudo apt-get update
sudo apt-get install build-essential git
```

Verify that `g++` supports C++20:

```bash
g++ --version   # should report version 10 or later
```

## Cloning the Repository

Use `git clone` to download the project:

```bash
git clone https://github.com/N3b3x/HF-AS5047U.git
cd HF-AS5047U
```

The rest of the documentation assumes you are inside the project directory.

---
⬅️ [Back to Documentation Index](README.md) | **Next:** [Building and Testing](building.md) ➡️
