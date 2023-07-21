#pragma once

#include "CesiumGltf/ExtensionExtMeshFeatures.h"
#include "CesiumGltf/ExtensionModelExtStructuralMetadata.h"
#include "CesiumGltf/Model.h"
#include "CesiumGltf/PropertyTypeTraits.h"
#include <glm/glm.hpp>
#include <type_traits>
#include <vector>

/**
 * @brief Converts the given vector of values into a std::vector of bytes.
 *
 * @returns The values as a std::vector of bytes.
 */
template <typename T>
std::vector<std::byte> GetValuesAsBytes(const std::vector<T>& values) {
  std::vector<std::byte> bytes(values.size() * sizeof(T));
  std::memcpy(bytes.data(), values.data(), bytes.size());

  return bytes;
}

/**
 * @brief Adds the buffer to the given model, creating a buffer view and
 * accessor in the process.
 * @returns The index of the accessor.
 */
int32_t AddBufferToModel(
    CesiumGltf::Model& model,
    const std::string& type,
    const int32_t componentType,
    const std::vector<std::byte>&& values);

/**
 * @brief Creates an attribute on the given primitive, including a buffer,
 * buffer view, and accessor for the given values.
 */
template <typename T>
void CreateAttributeForPrimitive(
    CesiumGltf::Model& model,
    CesiumGltf::MeshPrimitive& primitive,
    const std::string& attributeName,
    const std::string& type,
    const int32_t componentType,
    const std::vector<T>& values) {
  std::vector<std::byte> data = GetValuesAsBytes(values);
  const int32_t accessor =
      AddBufferToModel(model, type, componentType, std::move(data));
  primitive.attributes.insert({attributeName, accessor});
}

/**
 * @brief Creates indices for the given primitive, including a buffer,
 * buffer view, and accessor for the given values.
 */
template <typename T>
void CreateIndicesForPrimitive(
    CesiumGltf::Model& model,
    CesiumGltf::MeshPrimitive& primitive,
    const std::string& type,
    const int32_t componentType,
    const std::vector<T>& indices) {
  std::vector<std::byte> values = GetValuesAsBytes(indices);
  const int32_t accessor =
      AddBufferToModel(model, type, componentType, std::move(values));
  primitive.indices = accessor;
}

/**
 * @brief Adds the feature IDs to the given primitive as a feature ID attribute
 * in EXT_mesh_features. If the primitive doesn't already contain
 * EXT_mesh_features, this function adds it.
 *
 * @returns The newly created feature ID in the primitive extension.
 */
CesiumGltf::FeatureId& AddFeatureIDsAsAttributeToModel(
    CesiumGltf::Model& model,
    CesiumGltf::MeshPrimitive& primitive,
    const std::vector<uint8_t>& featureIDs,
    const int64_t featureCount,
    const int64_t setIndex);

/**
 * @brief Adds the feature IDs to the given primitive as a feature ID texture
 * in EXT_mesh_features. This also adds the given texcoords to the primitive as
 * a TEXCOORD attribute. If the primitive doesn't already contain
 * EXT_mesh_features, this function adds it.
 *
 * @returns The newly created feature ID in the primitive extension.
 */
CesiumGltf::FeatureId& AddFeatureIDsAsTextureToModel(
    CesiumGltf::Model& model,
    CesiumGltf::MeshPrimitive& primitive,
    const std::vector<uint8_t>& featureIDs,
    const int64_t featureCount,
    const int32_t imageWidth,
    const int32_t imageHeight,
    const std::vector<glm::vec2>& texCoords,
    const int64_t texcoordSetIndex);

/**
 * @brief Adds the given values to the given model as a
 * property table property in EXT_structural_metadata. This also creates a class
 * property definition for the new property in the schema. If the model doesn't
 * already contain EXT_structural_metadata, this function adds it.
 *
 * This assumes the given values are not arrays or strings.
 *
 * @returns The newly created property table property in the model extension.
 */
template <typename T>
CesiumGltf::PropertyTableProperty& AddPropertyTablePropertyToModel(
    CesiumGltf::Model& model,
    CesiumGltf::PropertyTable& propertyTable,
    const std::string& propertyName,
    const std::string& type,
    const std::optional<std::string>& componentType,
    const std::vector<T>& values) {
  ExtensionModelExtStructuralMetadata* pExtension =
      model.getExtension<ExtensionModelExtStructuralMetadata>();
  if (pExtension == nullptr) {
    pExtension = &model.addExtension<ExtensionModelExtStructuralMetadata>();
  }

  if (!pExtension->schema) {
    pExtension->schema.emplace();
  }
  Schema& schema = *pExtension->schema;

  const std::string& className = propertyTable.classProperty;
  CesiumGltf::Class& theClass = schema.classes[className];

  ClassProperty& classProperty = theClass.properties[propertyName];
  classProperty.type = type;
  classProperty.componentType = componentType;

  CesiumGltf::Buffer& buffer = model.buffers.emplace_back();
  buffer.cesium.data.resize(values.size() * sizeof(T));
  std::memcpy(
      buffer.cesium.data.data(),
      values.data(),
      buffer.cesium.data.size());
  buffer.byteLength = buffer.cesium.data.size();

  CesiumGltf::BufferView& bufferView = model.bufferViews.emplace_back();
  bufferView.buffer = static_cast<int32_t>(model.buffers.size() - 1);
  bufferView.byteLength = buffer.byteLength;
  bufferView.byteOffset = 0;

  CesiumGltf::PropertyTableProperty& property =
      propertyTable.properties[propertyName];
  property.values = static_cast<int32_t>(model.bufferViews.size() - 1);

  return property;
}