#include "Model.hpp"

using namespace Components;

Model::Model(std::shared_ptr<Storage::Manager> manager) : manager(manager) {
}

Model::~Model() = default;
