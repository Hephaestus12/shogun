/*
 * This software is distributed under BSD 3-clause license (see LICENSE file).
 * 
 * Both the documentation and the code is heavily inspired by pyGLMnet.: https://github.com/glm-tools/pyglmnet/
 *
 * Authors: Tej Sukhatme
 */


#include <shogun/lib/config.h>

#include <shogun/lib/common.h>
#include <shogun/machine/Machine.h>
#include <shogun/machine/LinearMachine.h>
#include <shogun/machine/IterativeMachine.h>
#include <shogun/mathematics/RandomMixin.h>
#include <shogun/distributions/Distribution.h>

namespace shogun
{
    enum GLM_DISTRIBUTION
	{
        //TODO GAUSSIAN,
        //TODO BINOMIAL,
        //TODO GAMMA,
        //TODO SOFTPLUS,
        //TODO PROBIT,
        POISSON
	};

class DotFeatures;
class Features;
class RegressionLabels;

/** @brief Class for estimating regularized generalized linear models (GLM).
 *   The regularized GLM minimizes the penalized negative log likelihood:
 *
 *  This used Elastic-net penalty which defaults to the ridge penalty when
 *  alpha = 0 and defaults to the lasso penalty when alpha = 1.
 *
 * */
class GLM : public RandomMixin<IterativeMachine<LinearMachine>>
{
 	public:
		friend class GLMCostFunction;

		GLM();

 		/** default constructor */
 		GLM(GLM_DISTRIBUTION distribution=POISSON, float64_t alpha=0.5, float64_t lambda=0.1, float64_t learning_rate=2e-1, int32_t max_iterations=1000, float64_t tolerance=1e-6, float64_t eta=2.0);

 		/** destructor */
 		~GLM() override {}

		/** apply linear machine to data
		 * for regression problem
		 *
		 * @param data (test)data to be classified
		 * @return classified labels
		 */
		std::shared_ptr<RegressionLabels> apply_regression(std::shared_ptr<Features> data=NULL) override;

 		/** Returns the name of the SGSerializable instance.  It MUST BE
 		 *  the CLASS NAME without the prefixed `C'.
 		 *
 		 * @return name of the SGSerializable
 		 */
 		const char* get_name() const override { return "GLM"; }

		void set_tau(SGMatrix<float64_t> tau);

		SGMatrix<float64_t> get_tau();

	protected:

		void init_model(const std::shared_ptr<Features>& data) override;
		
		void iteration() override;

	protected:

		/** Distribution type */
		GLM_DISTRIBUTION distribution = POISSON;

		/** a threshold parameter that linearizes the exp() function above eta. */
		float64_t m_eta = 2.0;

		/** regularization parameter :math:`\\lambda` of penalty term. */
		float64_t m_lambda = 0.1;

		/** the weighting between L1 penalty and L2 penalty term of the loss function. */
		float64_t m_alpha = 0.5;

		/** convergence threshold or stopping criteria. Optimization loop will stop when relative change in parameter norm is below the threshold. */
		float64_t m_tolerance = 1e-6;

		std::shared_ptr<GLMCostFunction> m_cost_function;

		std::shared_ptr<GradientDescendUpdater> m_gradient_updater;

		std::shared_ptr<ConstLearningRate> m_learning_rate;

		std::shared_ptr<ElasticNetPenalty> m_penalty;
};
}
// #endif
