// Copyright 2020-2021 CesiumGS, Inc. and Contributors

#include "CesiumMetadataPrimitive.h"
#include "CesiumGltf/ExtensionMeshPrimitiveExtFeatureMetadata.h"
#include "CesiumGltf/Model.h"

// REMOVE AFTER DEPRECATION
#include "CesiumGltf/ExtensionModelExtFeatureMetadata.h"

FCesiumMetadataPrimitive::FCesiumMetadataPrimitive(
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive,
    const CesiumGltf::ExtensionMeshPrimitiveExtFeatureMetadata& metadata,
    const CesiumGltf::ExtensionModelExtFeatureMetadata& modelMetadata) {

  const CesiumGltf::Accessor& indicesAccessor =
      model.getSafe(model.accessors, primitive.indices);
  switch (indicesAccessor.componentType) {
  case CesiumGltf::Accessor::ComponentType::UNSIGNED_BYTE:
    _vertexIDAccessor =
        CesiumGltf::AccessorView<CesiumGltf::AccessorTypes::SCALAR<uint8_t>>(
            model,
            indicesAccessor);
    break;
  case CesiumGltf::Accessor::ComponentType::UNSIGNED_SHORT:
    _vertexIDAccessor =
        CesiumGltf::AccessorView<CesiumGltf::AccessorTypes::SCALAR<uint16_t>>(
            model,
            indicesAccessor);
    break;
  case CesiumGltf::Accessor::ComponentType::UNSIGNED_INT:
    _vertexIDAccessor =
        CesiumGltf::AccessorView<CesiumGltf::AccessorTypes::SCALAR<uint32_t>>(
            model,
            indicesAccessor);
    break;
  default:
    break;
  }
  
  auto positionAccessorIt = primitive.attributes.find("POSITION");
  if (positionAccessorIt == primitive.attributes.end()) {
    // This primitive doesn't have a POSITION semantic, ignore it.
    return;
  }

  int positionAccessorID = positionAccessorIt->second;
  const CesiumGltf::Accessor* pPositionAccessor =
      CesiumGltf::Model::getSafe(&model.accessors, positionAccessorID);
  if (!pPositionAccessor) {
    // Position accessor does not exist, so ignore this primitive.
    return;
  }

  _positionAccessor =
      CesiumGltf::AccessorView<TMeshVector3>(model, *pPositionAccessor);

  for (const CesiumGltf::FeatureIDAttribute& attribute :
       metadata.featureIdAttributes) {
    if (attribute.featureIds.attribute) {
      auto featureID =
          primitive.attributes.find(attribute.featureIds.attribute.value());
      if (featureID == primitive.attributes.end()) {
        continue;
      }

      const CesiumGltf::Accessor* accessor =
          model.getSafe<CesiumGltf::Accessor>(
              &model.accessors,
              featureID->second);
      if (!accessor) {
        continue;
      }

      if (accessor->type != CesiumGltf::Accessor::Type::SCALAR) {
        continue;
      }

      this->_featureIdAttributes.Add(FCesiumFeatureIdAttribute(
          model,
          *accessor,
          featureID->second,
          UTF8_TO_TCHAR(attribute.featureTable.c_str())));

      // REMOVE AFTER DEPRECATION
      PRAGMA_DISABLE_DEPRECATION_WARNINGS
      auto featureTableIt =
          modelMetadata.featureTables.find(attribute.featureTable);
      if (featureTableIt == modelMetadata.featureTables.end()) {
        continue;
      }
      this->_featureTables_deprecated.Add((FCesiumMetadataFeatureTable(
          model,
          *accessor,
          featureTableIt->second)));
      PRAGMA_ENABLE_DEPRECATION_WARNINGS
    }
  }

  for (const CesiumGltf::FeatureIDTexture& featureIdTexture :
       metadata.featureIdTextures) {
    this->_featureIdTextures.Add(
        FCesiumFeatureIdTexture(model, featureIdTexture));
  }

  this->_featureTextureNames.Reserve(metadata.featureTextures.size());
  for (const std::string& featureTextureName : metadata.featureTextures) {
    this->_featureTextureNames.Emplace(
        UTF8_TO_TCHAR(featureTextureName.c_str()));
  }
}

PRAGMA_DISABLE_DEPRECATION_WARNINGS
const TArray<FCesiumMetadataFeatureTable>&
UCesiumMetadataPrimitiveBlueprintLibrary::GetFeatureTables(
    const FCesiumMetadataPrimitive& MetadataPrimitive) {
  return MetadataPrimitive._featureTables_deprecated;
}
PRAGMA_ENABLE_DEPRECATION_WARNINGS

const TArray<FCesiumFeatureIdAttribute>&
UCesiumMetadataPrimitiveBlueprintLibrary::GetFeatureIdAttributes(
    UPARAM(ref) const FCesiumMetadataPrimitive& MetadataPrimitive) {
  return MetadataPrimitive._featureIdAttributes;
}

const TArray<FCesiumFeatureIdTexture>&
UCesiumMetadataPrimitiveBlueprintLibrary::GetFeatureIdTextures(
    UPARAM(ref) const FCesiumMetadataPrimitive& MetadataPrimitive) {
  return MetadataPrimitive._featureIdTextures;
}

const TArray<FString>&
UCesiumMetadataPrimitiveBlueprintLibrary::GetFeatureTextureNames(
    UPARAM(ref) const FCesiumMetadataPrimitive& MetadataPrimitive) {
  return MetadataPrimitive._featureTextureNames;
}

int64 UCesiumMetadataPrimitiveBlueprintLibrary::GetFirstVertexIDFromFaceID(
    UPARAM(ref) const FCesiumMetadataPrimitive& MetadataPrimitive,
    int64 faceID) {
  return std::visit(
      [faceID](const auto& accessor) {
        int64 index = faceID * 3;

        if (accessor.status() != CesiumGltf::AccessorViewStatus::Valid) {
          // No indices, so each successive face is just the next three
          // vertices.
          return index;
        } else if (index >= accessor.size()) {
          // Invalid face index.
          return -1LL;
        }

        return int64(accessor[index].value[0]);
      },
      MetadataPrimitive._vertexIDAccessor);
}

int64 UCesiumMetadataPrimitiveBlueprintLibrary::GetSecondVertexIDFromFaceID(
    UPARAM(ref) const FCesiumMetadataPrimitive& MetadataPrimitive,
    int64 faceID) {
  return std::visit(
      [faceID](const auto& accessor) {
        int64 index = faceID * 3 +1 ;

        if (accessor.status() != CesiumGltf::AccessorViewStatus::Valid) {
          // No indices, so each successive face is just the next three
          // vertices.
          return index;
        } else if (index >= accessor.size()) {
          // Invalid face index.
          return -1LL;
        }

        return int64(accessor[index].value[0]);
      },
      MetadataPrimitive._vertexIDAccessor);
}

int64 UCesiumMetadataPrimitiveBlueprintLibrary::GetThirdVertexIDFromFaceID(
    UPARAM(ref) const FCesiumMetadataPrimitive& MetadataPrimitive,
    int64 faceID) {
  return std::visit(
      [faceID](const auto& accessor) {
        int64 index = faceID * 3 + 2;

        if (accessor.status() != CesiumGltf::AccessorViewStatus::Valid) {
          // No indices, so each successive face is just the next three
          // vertices.
          return index;
        } else if (index >= accessor.size()) {
          // Invalid face index.
          return -1LL;
        }

        return int64(accessor[index].value[0]);
      },
      MetadataPrimitive._vertexIDAccessor);
}


TMeshVector3
UCesiumMetadataPrimitiveBlueprintLibrary::GetFirstVertexPositionFromFaceID(
    UPARAM(ref) const FCesiumMetadataPrimitive& MetadataPrimitive,
    int64 faceID) {
  return std::visit(
      [faceID](const auto& accessor,const auto& pAccessor) {
        int64 index = faceID * 3;

        if (accessor.status() != CesiumGltf::AccessorViewStatus::Valid ||
            pAccessor.status() != CesiumGltf::AccessorViewStatus::Valid) {
          // No indices, so each successive face is just the next three
          // vertices.
          return TMeshVector3();
        } else if (index >= accessor.size()) {
          // Invalid face index.
          return TMeshVector3();
        }
        int64 pIndex = int64(accessor[index].value[0]);
        if (pIndex >= pAccessor.size()) {
          // Invalid face index.
          return TMeshVector3();
        }
        return TMeshVector3(pAccessor[pIndex]);
      },
      MetadataPrimitive._vertexIDAccessor,
      MetadataPrimitive._positionAccessor);
}

TMeshVector3
UCesiumMetadataPrimitiveBlueprintLibrary::GetSecondVertexPositionFromFaceID(
    UPARAM(ref) const FCesiumMetadataPrimitive& MetadataPrimitive,
    int64 faceID) {
  return std::visit(
      [faceID](const auto& accessor, const auto& pAccessor) {
        int64 index = faceID * 3 + 1;

        if (accessor.status() != CesiumGltf::AccessorViewStatus::Valid ||
            pAccessor.status() != CesiumGltf::AccessorViewStatus::Valid) {
          // No indices, so each successive face is just the next three
          // vertices.
          return TMeshVector3();
        } else if (index >= accessor.size()) {
          // Invalid face index.
          return TMeshVector3();
        }
        int64 pIndex = int64(accessor[index].value[0]);
        if (pIndex >= pAccessor.size()) {
          // Invalid face index.
          return TMeshVector3();
        }
        return TMeshVector3(pAccessor[pIndex]);
      },
      MetadataPrimitive._vertexIDAccessor,
      MetadataPrimitive._positionAccessor);
}

TMeshVector3
UCesiumMetadataPrimitiveBlueprintLibrary::GetThirdVertexPositionFromFaceID(
    UPARAM(ref) const FCesiumMetadataPrimitive& MetadataPrimitive,
    int64 faceID) {
  return std::visit(
      [faceID](const auto& accessor, const auto& pAccessor) {
        int64 index = faceID * 3 + 2;

        if (accessor.status() != CesiumGltf::AccessorViewStatus::Valid ||
            pAccessor.status() != CesiumGltf::AccessorViewStatus::Valid) {
          // No indices, so each successive face is just the next three
          // vertices.
          return TMeshVector3();
        } else if (index >= accessor.size()) {
          // Invalid face index.
          return TMeshVector3();
        }
        int64 pIndex = int64(accessor[index].value[0]);
        if (pIndex >= pAccessor.size()) {
          // Invalid face index.
          return TMeshVector3();
        }
        return TMeshVector3(pAccessor[pIndex]);
      },
      MetadataPrimitive._vertexIDAccessor,
      MetadataPrimitive._positionAccessor);
}

