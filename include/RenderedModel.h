#ifndef RENDEREDMODEL_H
#define RENDEREDMODEL_H

#include "RenderedTriangle.h"
#include <vector>
#include "BlockRenderedModel.h"

class RenderedModel {
    public:
        RenderedModel(std::vector<RenderedTriangle> _renderedModel);

        std::vector<RenderedTriangle> renderedModel = std::vector<RenderedTriangle>();
};
#endif