#include <shogun/labels/StructuredLabels.h>
#include <shogun/structure/HMSVMModel.h>
#include <shogun/structure/PrimalMosekSOSVM.h>
#include <shogun/structure/TwoStateModel.h>

using namespace shogun;

int main(int argc, char ** argv)
{
#ifdef USE_MOSEK

	int32_t num_examples = 10;
	int32_t example_length = 250;
	int32_t num_features = 10;
	int32_t num_noise_features = 2;
	CHMSVMModel* model = TwoStateModel::simulate_data(num_examples, example_length, num_features, num_noise_features);

	StructuredLabels* labels = model->get_labels();
	Features* features = model->get_features();

	CPrimalMosekSOSVM* sosvm = new CPrimalMosekSOSVM(model, labels);

	sosvm->train();
//	sosvm->get_w().display_vector("w");

	StructuredLabels* out = sosvm->apply()->as<StructuredLabels>();

	ASSERT( out->get_num_labels() == labels->get_num_labels() );

	for ( int32_t i = 0 ; i < out->get_num_labels() ; ++i )
	{
		CSequence* pred_seq = CSequence::obtain_from_generic( out->get_label(i) );
		CSequence* true_seq = CSequence::obtain_from_generic( labels->get_label(i) );
	}


#endif /* USE_MOSEK */

	return 0;
}
