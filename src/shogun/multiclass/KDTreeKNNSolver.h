/* This software is distributed under BSD 3-clause license (see LICENSE file).
 *
 * Copyright (c) 2012-2013 Sergey Lisitsyn
 */

#ifndef KDTREESOLVER_H__
#define KDTREESOLVER_H__

#include <shogun/lib/config.h>

#include <shogun/lib/common.h>
#include <shogun/distance/Distance.h>
#include <shogun/multiclass/KNNSolver.h>
#include <shogun/multiclass/tree/KDTree.h>

namespace shogun
{

/**
 * KD-tree solver. It uses k-d tree (short for k-dimensional tree) to speed up the 
 * nearest neighbour computation. 
 * For more information, see https://en.wikipedia.org/wiki/K-d_tree
 *
 */
class KDTREEKNNSolver : public KNNSolver
{
	public:
		/** default constructor */
		KDTREEKNNSolver() : KNNSolver()
		{
			init(); 
		}

		/** deconstructor */
		virtual ~KDTREEKNNSolver() { /* nothing to do */ }

		/** constructor
		 *
		 * @param k k
		 * @param q m_q
		 * @param num_classes m_num_classes
		 * @param min_label m_min_label
		 * @param train_labels m_train_labels
		 * @param leaf_size m_leaf_size
		 */
		KDTREEKNNSolver(const int32_t k, const float64_t q, const int32_t num_classes, const int32_t min_label, const SGVector<int32_t> train_labels, const int32_t leaf_size);

		virtual std::shared_ptr<MulticlassLabels> classify_objects(std::shared_ptr<Distance> d, const int32_t num_lab, SGVector<int32_t>& train_lab, SGVector<float64_t>& classes);

		virtual SGVector<int32_t> classify_objects_k(std::shared_ptr<Distance> d, const int32_t num_lab, SGVector<int32_t>& train_lab, SGVector<int32_t>& classes);

		virtual bool compute_nearest_neighbours(std::shared_ptr<Distance> knn_distance);

		virtual bool train_KNN(std::shared_ptr<Distance> knn_distance);

		/** @return object name */
		const char* get_name() const { return "KDTREEKNNSolver"; }

	private:
		void init()
		{
			m_leaf_size=0;
		}

	protected:
		// leaf size of K-D tree
		int32_t m_leaf_size;

		//instance of K-D tree
		std::shared_ptr<shogun::KDTree> m_kd_tree;
};
}

#endif
