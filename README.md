<img src=http://storage.mds.yandex.net/get-devtools-opensource/250854/catboost-logo.png width=300/>

[Website](https://catboost.ai) |
[Documentation](https://catboost.ai/docs/) |
[Tutorials](https://catboost.ai/docs/concepts/tutorials.html) |
[Installation](https://catboost.ai/docs/concepts/installation.html) |
[Release Notes](https://github.com/catboost/catboost/releases)

[![GitHub license](https://img.shields.io/github/license/catboost/catboost.svg)](https://github.com/catboost/catboost/blob/master/LICENSE)
[![PyPI version](https://badge.fury.io/py/catboost.svg)](https://badge.fury.io/py/catboost)
[![Conda Version](https://img.shields.io/conda/vn/conda-forge/catboost.svg)](https://anaconda.org/conda-forge/catboost)
[![GitHub issues](https://img.shields.io/github/issues/catboost/catboost.svg)](https://github.com/catboost/catboost/issues)
[![Telegram](https://img.shields.io/badge/chat-on%20Telegram-2ba2d9.svg)](https://t.me/catboost_en)
[![Twitter](https://img.shields.io/badge/@CatBoostML--_.svg?style=social&logo=twitter)](https://twitter.com/CatBoostML)

## Exploratory fork (`exploratory-1.2.10`)

This branch is catboost **v1.2.10** plus one change, used by [Exploratory](https://exploratory.io) (R package version **1.2.10.1**):

- New binary-classification metric `AveragePrecision` (`ELossFunction::AveragePrecision`), equal to scikit-learn `average_precision_score`: the step sum `sum((R_n - R_{n-1}) * P_n)` with tied scores grouped. It is maximized (best value 1), supports `use_weights`, and skips train-set evaluation by default (like `PRAUC`). Use it as `eval_metric="AveragePrecision"`. The learning-curve column in `test_error.tsv` is `AveragePrecision`.
- Code: `catboost/libs/metrics/metric.cpp` (`TAveragePrecisionMetric`), `catboost/private/libs/options/{enums.h,enum_helpers.cpp}`, unit test `catboost/libs/metrics/ut/average_precision_ut.cpp`.
- `PRAUC` is untouched, and so are the model format and all other metrics: predictions for the same seed and data are identical to 1.2.10.

### Build recipe (R package only, `--targets catboostr`)

Common: CMake **< 4** (3.24-3.31), Conan **2.x**, Ninja, Python 3, clang. Run `build/build_native.py` from the repo root.

- **macOS universal2** (needs Xcode or Command Line Tools with a recent SDK; built on macOS 26):

  ```bash
  python build/build_native.py --targets catboostr --build-root-dir=./b --macos-universal-binaries --macosx-version-min=11.0
  # result: b/catboost/R-package/src/libcatboostr.dylib (fat x86_64 + arm64, minos 11.0)
  ```

- **Linux x86_64**: build in the same base image as the R Docker image (`rocker/r-ver:4.6.1`, Ubuntu 24.04), after `apt-get install clang lld python3-venv python3-dev git ninja-build make binutils` and `pip install "cmake<4" "conan>=2,<3"`:

  ```bash
  python3 build/build_native.py --targets catboostr --build-root-dir=/work/b   # ~10 min on 16 cores
  # result: /work/b/catboost/R-package/src/libcatboostr.so (needs GLIBC >= 2.38)
  ```

- **Windows x64**: VS 2022 Build Tools with the clang-cl components and the MSVC v14.29 toolset
  (`Microsoft.VisualStudio.Component.VC.14.29.16.11.x86.x64`, the `build_native.py` default), Python 3.12, CMake < 4, Conan 2, Ninja **1.12.1** (from the GitHub release; PyPI has no such wheel). `build_native.py` only looks for `Community`/`Enterprise`, so for Build Tools make a junction first:

  ```bat
  mklink /J C:\vs\2022\Community "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools"
  python build\build_native.py --targets catboostr --build-root-dir=C:\cb\b --msvs-installation-path=C:\vs
  :: result: C:\cb\b\catboost\R-package\src\catboostr.dll
  ```

### Packaging

Copy `DESCRIPTION NAMESPACE README.md R inst man tests` from `catboost/R-package` into a `catboost/` directory, put the library at `catboost/inst/libs/libcatboostr.so` (macOS: also a `libcatboostr.dylib` symlink; Windows: `inst/libs/x64/libcatboostr.dll`), tar it as in `ci/build_all.py` (`build_r_package`), then turn it into an installed-package binary with `R CMD INSTALL --build --no-staged-install` (`--no-staged-install` is needed on macOS, where R's staged-install path check rejects the library). The result is what Exploratory publishes: `catboost_1.2.10.1.tgz` (macOS), `catboost_1.2.10.1.zip` (Windows), `catboost_1.2.10.1_R_x86_64-pc-linux-gnu.tar.gz` (Linux, Docker image).

### Verification used

`eval_metric="AveragePrecision"` trains and early-stops in the maximize direction; its last validation value equals `sklearn.metrics.average_precision_score` on the validation predictions (to 1e-10), including ties and `use_weights`; `AUC` predictions with categorical columns are byte-identical to the 1.2.10 binary for the same seed.

CatBoost is a machine learning method based on [gradient boosting](https://en.wikipedia.org/wiki/Gradient_boosting) over decision trees.

Main advantages of CatBoost:
--------------
  - Superior quality when [compared](https://github.com/catboost/benchmarks/blob/master/README.md) with other GBDT libraries on many datasets.
  - Best in class [prediction](https://catboost.ai/docs/concepts/c-plus-plus-api.html) speed.
  - Support for both [numerical and categorical](https://catboost.ai/docs/concepts/algorithm-main-stages.html) features.
  - Fast GPU and multi-GPU support for training out of the box.
  - Visualization tools [included](https://catboost.ai/docs/features/visualization.html).
  - Fast and reproducible distributed training with [Apache Spark](https://catboost.ai/en/docs/concepts/spark-overview) and [CLI](https://catboost.ai/en/docs/concepts/cli-distributed-learning).

Get Started and Documentation
--------------
All CatBoost documentation is available [here](https://catboost.ai/docs/).

Install CatBoost by following the guide for the
 * [Python package](https://catboost.ai/en/docs/concepts/python-installation)
 * [R-package](https://catboost.ai/en/docs/concepts/r-installation)
 * [Сommand line](https://catboost.ai/en/docs/concepts/cli-installation)
 * [Package for Apache Spark](https://catboost.ai/en/docs/concepts/spark-installation)

Next you may want to investigate:
* [Tutorials](https://github.com/catboost/tutorials/#readme)
* [Training modes and metrics](https://catboost.ai/docs/concepts/loss-functions.html)
* [Cross-validation](https://catboost.ai/docs/features/cross-validation.html#cross-validation)
* [Parameters tuning](https://catboost.ai/docs/concepts/parameter-tuning.html)
* [Feature importance calculation](https://catboost.ai/docs/features/feature-importances-calculation.html)
* [Regular](https://catboost.ai/docs/features/prediction.html#prediction) and [staged](https://catboost.ai/docs/features/staged-prediction.html#staged-prediction) predictions
* CatBoost for Apache Spark videos: [Introduction](https://youtu.be/47-mAVms-b8) and [Architecture](https://youtu.be/nrGt5VKZpzc)

If you cannot open documentation in your browser try adding yastatic.net and yastat.net to the list of allowed domains in your privacy badger.

CatBoost models in production
--------------
If you want to evaluate CatBoost model in your application read [model api documentation](https://github.com/catboost/catboost/tree/master/catboost/CatboostModelAPI.md).

Questions and bug reports
--------------
* For reporting bugs please use the [catboost/bugreport](https://github.com/catboost/catboost/issues) page.
* Ask a question on [CatBoost GitHub Discussions Q&A forum](https://github.com/catboost/catboost/discussions/categories/q-a).
* Ask a question on [Stack Overflow](https://stackoverflow.com/questions/tagged/catboost) with the catboost tag, we monitor this for new questions.
* Seek prompt advice at [Telegram group](https://t.me/catboost_en) or Russian-speaking [Telegram chat](https://t.me/catboost_ru)

Help to Make CatBoost Better
----------------------------
* Check out [open problems](https://github.com/catboost/catboost/blob/master/open_problems/open_problems.md) and [help wanted issues](https://github.com/catboost/catboost/labels/help%20wanted) to see what can be improved, or open an issue if you want something.
* Add your stories and experience to [Awesome CatBoost](AWESOME.md).
* [Instructions for contributors](https://github.com/catboost/catboost/blob/master/CONTRIBUTING.md).

News
--------------
Latest news are published on [twitter](https://twitter.com/catboostml).

Reference Paper
-------
Anna Veronika Dorogush, Andrey Gulin, Gleb Gusev, Nikita Kazeev, Liudmila Ostroumova Prokhorenkova, Aleksandr Vorobev ["Fighting biases with dynamic boosting"](https://arxiv.org/abs/1706.09516). arXiv:1706.09516, 2017.

Anna Veronika Dorogush, Vasily Ershov, Andrey Gulin ["CatBoost: gradient boosting with categorical features support"](http://learningsys.org/nips17/assets/papers/paper_11.pdf). Workshop on ML Systems
at NIPS 2017.

License
-------
© YANDEX LLC, 2017-2026. Licensed under the Apache License, Version 2.0. See LICENSE file for more details.
