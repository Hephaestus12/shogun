/*
 * This software is distributed under BSD 3-clause license (see LICENSE file).
 *
 * Both the documentation and the code is heavily inspired by pyGLMnet.:
 * https://github.com/glm-tools/pyglmnet/
 *
 * Author: Tej Sukhatme
 */

#include <gtest/gtest.h>
#include <shogun/features/DenseFeatures.h>
#include <shogun/labels/RegressionLabels.h>
#include <shogun/lib/SGMatrix.h>
#include <shogun/machine/GLM.h>
#include <shogun/machine/LinearMachine.h>
#include <shogun/mathematics/RandomNamespace.h>
#include <shogun/mathematics/UniformRealDistribution.h>
#include <shogun/mathematics/eigen3.h>
#include <shogun/mathematics/linalg/LinalgNamespace.h>
#include <shogun/preprocessor/NormOne.h>
#include <shogun/preprocessor/PruneVarSubMean.h>
#include <random>

using namespace shogun;

// Generate the Data that N is greater than D
std::tuple<SGMatrix<float64_t>, SGVector<float64_t>> generate_train_data()
{
	SGVector<float64_t> labels = SGVector<float64_t>(
	    {7.23514031, 7.23514031, 7.23514031, 7.23514031, 7.23514031});
	SGMatrix<float64_t> features =
	    SGMatrix<float64_t>({{0.71307143, -0.67054885, -0.24406853},
	                         {-0.79774475, -1.65627891, 0.95675428},
	                         {0.96709333, 1.81672959, 0.20911922},
	                         {2.1912712, 0.23820139, 1.07501177},
	                         {-0.58427793, -0.61855905, 1.27687684}});

	return {features, labels};
}

std::tuple<SGMatrix<float64_t>, SGVector<float64_t>> generate_test_data()
{
	SGVector<float64_t> labels = SGVector<float64_t>(
	    {7.23514031, 7.23514031, 7.23514031, 7.23514031, 7.23514031});
	SGMatrix<float64_t> features =
	    SGMatrix<float64_t>({{1.26465769, 0.05451801, -0.21206714},
	                         {-0.3447881, -0.81339926, 1.636931},
	                         {0.3967461, -1.6470009, 0.89995864},
	                         {0.65379594, 1.08610417, -0.04911578},
	                         {0.6573247, -0.1306287, -0.64715244}});

	return {features, labels};
}

TEST(GLM, GLM_basic_test)
{
	const auto& [Xtrain, ytrain] = generate_train_data();
	const auto& [Xtest, ytest] = generate_test_data();

	auto features_train = std::make_shared<DenseFeatures<float64_t>>(Xtrain);
	auto labels_train = std::make_shared<RegressionLabels>(ytrain);

	auto features_test = std::make_shared<DenseFeatures<float64_t>>(Xtest);

	auto glm = std::make_shared<GLM>(POISSON, 0.5, 0.1, 2e-1, 1000, 1e-6, 2.0);

	glm->set_bias(0.44101309);
	glm->set_w(SGVector<float64_t>({0.1000393, 0.2446845, 0.5602233}));

	glm->set_labels(labels_train);

	glm->train(features_train);

	auto labels_predict = glm->apply_regression(features_test);

	SGVector<float64_t> labels_pyglmnet(
	    {5.47606254, 5.62436215, 5.50835709, 5.75757967, 6.01670904});

	float64_t epsilon = 1e-7;

	for (index_t i = 0; i < labels_predict->get_num_labels(); ++i)
		EXPECT_NEAR(labels_predict->get_label(i), labels_pyglmnet[i], epsilon);
}

TEST(GLMCostFunction, GLM_POISSON_gradient_test)
{
	const auto& [Xtrain, ytrain] = generate_train_data();

	auto glm_cost = std::make_shared<GLMCostFunction>();

	float64_t bias = -0.347144;
	SGVector<float64_t> w =
	    SGVector<float64_t>({0.295102, 0.423805, -0.125878});

	SGVector<float64_t> grad_w = glm_cost->get_gradient_weights(
	    Xtrain, ytrain, w, bias, 0.1, 0.5, true, 2.0, POISSON);
	float64_t grad_bias = glm_cost->get_gradient_bias(
	    Xtrain, ytrain, w, bias, true, 2.0, POISSON);

	/* gradient calculated here:
	 * https://gist.github.com/Hephaestus12/84ace46a18deed6157dca0a3e3640bfe */

	SGVector<float64_t> pyglmnet_grad_w =
	    SGVector<float64_t>({-2.62183289, 1.87007061, -4.26700914});
	float64_t pyglmnet_grad_bias = -6.31650804;
	float64_t epsilon = 1e-7;

	for (auto i : range(grad_w.vlen))
		EXPECT_NEAR(grad_w[i], pyglmnet_grad_w[i], epsilon);

	EXPECT_NEAR(grad_bias, pyglmnet_grad_bias, epsilon);
}