#include "text.h"

Text::Text() {}
Text::~Text() {}

/*-----------------------------------------------------------------------------
------------------------------INITIALIZATION-----------------------------------
-----------------------------------------------------------------------------*/
void Text::init(const GfxAccess& access) {
    util::log("Initializing text overlay..");

    access_ = access;

    assert(access_.vkRenderpass != VK_NULL_HANDLE);

    prepareResources();
    preparePipeline();
}


void Text::prepareResources() {
    util::log("preparing resources for text overlay...");

    // create texture to sample from
    fontTexture_.create("../res/font/font.png", *access_.dvcePtr, *access_.cmdrPtr);
    
    // Vertex buffer
    VkDeviceSize bufferSize = TEXTOVERLAY_MAX_CHAR_COUNT * sizeof(glm::vec4);

    util::createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        buffer_, memory_, access_.dvcePtr->getLogical(),
        access_.dvcePtr->getPhysical());

    
    // Descriptor
    // Font uses a separate descriptor pool
    util::log("Creating text overlay descriptor pool...");
    std::array<VkDescriptorPoolSize, 1> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[0].descriptorCount = 1;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 1;

    if (vkCreateDescriptorPool(access_.dvcePtr->getLogical(), &poolInfo, nullptr, &descriptorPool_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }

    // descriptor set layout:
    util::log("Creating text overlay descriptor set layout...");
    std::array<VkDescriptorSetLayoutBinding, 1> setLayoutBindings;
    setLayoutBindings[0].binding = 0;
    setLayoutBindings[0].descriptorCount = 1;
    setLayoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    setLayoutBindings[0].pImmutableSamplers = nullptr;
    setLayoutBindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;


    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
    layoutInfo.pBindings = setLayoutBindings.data();

    if (vkCreateDescriptorSetLayout(access_.dvcePtr->getLogical(), &layoutInfo, nullptr, &descriptorSetLayout_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
    
    // Descriptor set
    util::log("Creating text overlay descriptor set...");
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool_;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &descriptorSetLayout_;

    if (vkAllocateDescriptorSets(access_.dvcePtr->getLogical(), &allocInfo, &descriptorSet_) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    // Descriptor for the font image
    VkDescriptorImageInfo textureDescriptor;
    textureDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    textureDescriptor.imageView = fontTexture_.getTextureImageView();
    textureDescriptor.sampler = fontTexture_.getTextureSampler();

    std::array<VkWriteDescriptorSet, 1> descriptorWrites{};
    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = descriptorSet_;
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pImageInfo = &textureDescriptor;

    vkUpdateDescriptorSets(access_.dvcePtr->getLogical(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

// Prepare a separate pipeline for the font rendering decoupled from the main application
void Text::preparePipeline() {
    util::log("preparing text overlay pipeline...");

    util::log("Loading text overlay shaders..");

    auto vertShaderCode = util::readFile("../shaders/compiled/text_vert.spv");
    auto fragShaderCode = util::readFile("../shaders/compiled/text_frag.spv");

    VkShaderModule vertShaderModule = util::createShaderModule(vertShaderCode, access_.dvcePtr->getLogical());
    VkShaderModule fragShaderModule = util::createShaderModule(fragShaderCode, access_.dvcePtr->getLogical());

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo,
                                                    fragShaderStageInfo };

    // Pipeline cache
    VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
    pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    if (vkCreatePipelineCache(access_.dvcePtr->getLogical(), &pipelineCacheCreateInfo, nullptr, &pipelineCache_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline cache!");
    }

    // Layout
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout_;

    if (vkCreatePipelineLayout(access_.dvcePtr->getLogical(), &pipelineLayoutInfo, nullptr, &pipelineLayout_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    // Enable blending, using alpha from red channel of the font texture (see text.frag)
    VkPipelineColorBlendAttachmentState blendAttachmentState{};
    blendAttachmentState.blendEnable = VK_TRUE;
    blendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    blendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    blendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    blendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
    blendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    blendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    blendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState{};
    inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP; // IMPORTANT!!!
    inputAssemblyState.primitiveRestartEnable = VK_FALSE;

    VkPipelineRasterizationStateCreateInfo rasterizationState{};
    rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.depthClampEnable = VK_FALSE;
    rasterizationState.rasterizerDiscardEnable = VK_FALSE;
    rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationState.lineWidth = 1.0f;
    rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizationState.depthBiasEnable = VK_FALSE;
 
    VkPipelineColorBlendStateCreateInfo colorBlendState{};
    colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendState.logicOpEnable = VK_FALSE;
    colorBlendState.logicOp = VK_LOGIC_OP_COPY;
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments = &blendAttachmentState;
    //colorBlendState.blendConstants[0] = 0.0f; // v Keep?
    //colorBlendState.blendConstants[1] = 0.0f; 
    //colorBlendState.blendConstants[2] = 0.0f;
    //colorBlendState.blendConstants[3] = 0.0f;

    VkPipelineDepthStencilStateCreateInfo depthStencilState{};
    depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilState.depthTestEnable = VK_FALSE;
    depthStencilState.depthWriteEnable = VK_FALSE;
    depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    depthStencilState.depthBoundsTestEnable = VK_FALSE;
    depthStencilState.stencilTestEnable = VK_FALSE;
    
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;


    VkPipelineMultisampleStateCreateInfo multisampleState{};
    multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.sampleShadingEnable = VK_FALSE; // remove?
    multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    std::vector<VkDynamicState> dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
    dynamicState.pDynamicStates = dynamicStateEnables.data();

    auto vertexInputBindings = LetterVertex::getBindingDescription(); // FIXME does there need to be 2 of these?
    auto vertexInputAttributes = LetterVertex::getAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputState{};
    vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputState.vertexBindingDescriptionCount = 1;
    vertexInputState.pVertexBindingDescriptions = &vertexInputBindings;
    vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttributes.size());
    vertexInputState.pVertexAttributeDescriptions = vertexInputAttributes.data();

    VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.layout = pipelineLayout_;
    pipelineCreateInfo.renderPass = renderPass_;
    pipelineCreateInfo.pVertexInputState = &vertexInputState;
    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
    pipelineCreateInfo.pRasterizationState = &rasterizationState;
    pipelineCreateInfo.pColorBlendState = &colorBlendState;
    pipelineCreateInfo.pMultisampleState = &multisampleState;
    pipelineCreateInfo.pViewportState = &viewportState;
    pipelineCreateInfo.pDepthStencilState = &depthStencilState;
    pipelineCreateInfo.pDynamicState = &dynamicState;
    pipelineCreateInfo.stageCount = 2;
    pipelineCreateInfo.pStages = shaderStages;

    if (vkCreateGraphicsPipelines(access_.dvcePtr->getLogical(), pipelineCache_, 1, &pipelineCreateInfo, nullptr, &pipeline_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(access_.dvcePtr->getLogical(), fragShaderModule, nullptr);
    vkDestroyShaderModule(access_.dvcePtr->getLogical(), vertShaderModule, nullptr);
}


/*-----------------------------------------------------------------------------
------------------------------UPDATE-------------------------------------------
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
------------------------------DRAW---------------------------------------------
-----------------------------------------------------------------------------*/
void Text::draw(VkCommandBuffer commandBuffer) {
    
}


/*-----------------------------------------------------------------------------
------------------------------CLEANUP------------------------------------------
-----------------------------------------------------------------------------*/
void Text::cleanup() {
    util::log("Cleaning up text overlay object..");
    // Free up all Vulkan resources requested by the text overlay
    // FIXME
    fontTexture_.cleanup();

    // vertex buffer
    vkDestroyBuffer(access_.dvcePtr->getLogical(), buffer_, nullptr);
    vkFreeMemory(access_.dvcePtr->getLogical(), memory_, nullptr);

    vkDestroyDescriptorSetLayout(access_.dvcePtr->getLogical(), descriptorSetLayout_, nullptr);
    vkDestroyDescriptorPool(access_.dvcePtr->getLogical(), descriptorPool_, nullptr);
    vkDestroyPipelineLayout(access_.dvcePtr->getLogical(), pipelineLayout_, nullptr);
    vkDestroyPipelineCache(access_.dvcePtr->getLogical(), pipelineCache_, nullptr);
    vkDestroyPipeline(access_.dvcePtr->getLogical(), pipeline_, nullptr);

}
