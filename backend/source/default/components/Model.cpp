#include "Model.hpp"

using namespace Components;

Model::Model(Storage::Manager& storageManager) : storageManager(storageManager) {
}

Model::~Model() = default;
