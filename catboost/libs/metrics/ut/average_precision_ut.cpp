#include <library/cpp/testing/unittest/registar.h>
#include <catboost/libs/metrics/metric.h>
#include <catboost/libs/metrics/metric_holder.h>

//The benchmark values were calculated by sklearn.metrics.average_precision_score
Y_UNIT_TEST_SUITE(AveragePrecisionMetricTest) {
    Y_UNIT_TEST(BinaryClassification) {

        TVector<TVector<double>> approx{{-2, -1, -0.5, 0, 0.5, 1, 2}};
        TVector<float> target{0, 1, 0, 1, 0, 1, 1};
        TVector<float> weight;
        NPar::TLocalExecutor executor;

        const auto metric = MakeBinClassAveragePrecisionMetric(TLossParams());
        TMetricHolder score = dynamic_cast<const ISingleTargetEval*>(metric.Get())->Eval(approx, target, weight, {}, 0, target.size(), executor);

        UNIT_ASSERT_DOUBLES_EQUAL(metric->GetFinalError(score), 0.8541666666666666, 1e-9);
    }
    Y_UNIT_TEST(BinaryClassification_EqualApproxes) {

        TVector<TVector<double>> approx{{0.1, 0.1, 0.1, 0.2, 0.2, 0.3, 0.4, 0.5, 0.5, 0.6}};
        TVector<float> target{1, 0, 0, 1, 0, 0, 0, 0, 1, 1};
        TVector<float> weight;
        NPar::TLocalExecutor executor;

        const auto metric = MakeBinClassAveragePrecisionMetric(TLossParams());
        TMetricHolder score = dynamic_cast<const ISingleTargetEval*>(metric.Get())->Eval(approx, target, weight, {}, 0, target.size(), executor);

        UNIT_ASSERT_DOUBLES_EQUAL(metric->GetFinalError(score), 0.6238095238095238, 1e-9);
    }
    Y_UNIT_TEST(BinaryClassification_Weighted) {

        TVector<TVector<double>> approx{{0.1, 0.1, 0.1, 0.2, 0.2, 0.3, 0.4, 0.5, 0.5, 0.6}};
        TVector<float> target{1, 0, 0, 1, 0, 0, 0, 0, 1, 1};
        TVector<float> weight{0.5, 0.3, 1, 0.9, 1, 2, 0, 0.1, 1, 0.7};
        NPar::TLocalExecutor executor;

        const auto metric = MakeBinClassAveragePrecisionMetric(TLossParams());
        metric->UseWeights = true;
        TMetricHolder score = dynamic_cast<const ISingleTargetEval*>(metric.Get())->Eval(approx, target, weight, {}, 0, target.size(), executor);

        UNIT_ASSERT_DOUBLES_EQUAL(metric->GetFinalError(score), 0.7295604602905112, 1e-6);
    }
    Y_UNIT_TEST(BinaryClassification_SubRange) {

        TVector<TVector<double>> approx{{0.1, 0.1, 0.1, 0.2, 0.2, 0.3, 0.4, 0.5, 0.5, 0.6}};
        TVector<float> target{1, 0, 0, 1, 0, 0, 0, 0, 1, 1};
        TVector<float> weight;
        NPar::TLocalExecutor executor;

        const auto metric = MakeBinClassAveragePrecisionMetric(TLossParams());
        TMetricHolder score = dynamic_cast<const ISingleTargetEval*>(metric.Get())->Eval(approx, target, weight, {}, 2, 9, executor);

        UNIT_ASSERT_DOUBLES_EQUAL(metric->GetFinalError(score), 0.41666666666666663, 1e-9);
    }
    Y_UNIT_TEST(BinaryClassification_Perfect) {

        TVector<TVector<double>> approx{{0.1, 0.2, 0.3, 0.4}};
        TVector<float> target{0, 0, 1, 1};
        TVector<float> weight;
        NPar::TLocalExecutor executor;

        const auto metric = MakeBinClassAveragePrecisionMetric(TLossParams());
        TMetricHolder score = dynamic_cast<const ISingleTargetEval*>(metric.Get())->Eval(approx, target, weight, {}, 0, target.size(), executor);

        UNIT_ASSERT_DOUBLES_EQUAL(metric->GetFinalError(score), 1.0, 1e-9);
    }
}
