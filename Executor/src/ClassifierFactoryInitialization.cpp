#include "ClassifierFactoryInitialization.h"

#include <numeric>

#include <ClassifierFactory.h>
#include <Classifier.h>
#include <LinearClassifier.h>
#include <LASSOClassifier.h>
#include <NearestNeighborClassifier.h>
#include <NearestNeighborsStrategies.h>
#include <RandomClassifier.h>

#include <ILAClassifier.h>
#include <EqualFrequencyDiscretizer.h>
#include <EqualSizeDiscretizer.h>

namespace setup
{
	ClassifierFactory* classifier_factory(const std::string& defaultClassifier, std::shared_ptr<RandomServer> randomness)
	{
		ClassifierFactory* factory = new ClassifierFactory;
		factory->registerBuilder("Random", std::shared_ptr<Classifier::Builder>(new RandomClassifier::Builder(randomness)));
		factory->registerBuilder("ILA_EqualSizes_3", std::make_shared<ILAClassifier::Builder>(EqualSizeDiscretizer::creator, 3));
		factory->registerBuilder("ILA_EqualSizes_5", std::make_shared<ILAClassifier::Builder>(EqualSizeDiscretizer::creator, 5));
		factory->registerBuilder("ILA_EqualSizes_7", std::make_shared<ILAClassifier::Builder>(EqualSizeDiscretizer::creator, 7));
		factory->registerBuilder("ILA_EqualSizes_10", std::make_shared<ILAClassifier::Builder>(EqualSizeDiscretizer::creator, 10));
		factory->registerBuilder("ILA_EqualFrequencies_3", std::make_shared<ILAClassifier::Builder>(EqualFrequencyDiscretizer::creator, 3));
		factory->registerBuilder("ILA_EqualFrequencies_5", std::make_shared<ILAClassifier::Builder>(EqualFrequencyDiscretizer::creator, 5));
		factory->registerBuilder("ILA_EqualFrequencies_7", std::make_shared<ILAClassifier::Builder>(EqualFrequencyDiscretizer::creator, 7));
		factory->registerBuilder("ILA_EqualFrequencies_10", std::make_shared<ILAClassifier::Builder>(EqualFrequencyDiscretizer::creator, 10));

#ifdef BUILD_WITH_SHARK
		factory->registerBuilder("Linear", std::shared_ptr<Classifier::Builder>(new LinearClassifier::Builder));
		factory->registerBuilder("NearestNeighbor_StandardTree", std::shared_ptr<Classifier::Builder>(new NearestNeighborClassifier::Builder(new StandardTreeStrategy(1))));
		factory->registerBuilder("LASSO", std::shared_ptr<Classifier::Builder>(new LASSOClassifier::Builder()));
#endif

		auto available = factory->availableClassifiers();

		if (available.count(defaultClassifier) == 0)
			throw std::out_of_range(std::accumulate(available.begin(), available.end(), std::string("Classifier: ") + defaultClassifier + " is not available. Available classifiers: \n",
				[](const std::string& acc, const std::string& app){return acc +"-> " + app + "\n"; }));

		factory->setDefaultBuilder(defaultClassifier);
		return factory;
	}
}