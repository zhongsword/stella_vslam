#ifndef STELLA_VSLAM_FEATURE_XFEAT_PARAMS_H
#define STELLA_VSLAM_FEATURE_XFEAT_PARAMS_H

#include <nlohmann/json_fwd.hpp>
#include <yaml-cpp/yaml.h>
#include <vector>
#include <cmath>

namespace stella_vslam {
namespace feature {

struct xfeat_params {
    xfeat_params() = delete;

    //! Constructor
    xfeat_params(const std::string& name,
                 const unsigned int max_keypoints,
                 const float conf_th,
                 const std::vector<float>& scale_factors = {1.0f});

    //! Load from YAML
    explicit xfeat_params(const YAML::Node& yaml_node);

    //! Destructor
    virtual ~xfeat_params() = default;

    //! Export to JSON
    nlohmann::json to_json() const;

    //! Identifier (for saving/loading)
    const std::string name_;

    //! 可调超参数
    const unsigned int max_keypoints_ = 4096;
    const float conf_th_              = 0.25f;

    //! 半密集额外尺度（仅 XFeat* 使用）
    const std::vector<float> scale_factors_;

    //! 预计算表（与 ORB 保持一致接口）
    std::vector<float> scale_values_;
    std::vector<float> inv_scale_values_;
    std::vector<float> level_sigma_sq_;
    std::vector<float> inv_level_sigma_sq_;

    //! 工具函数
    static std::vector<float> calc_scale_values(const std::vector<float>& factors);
    static std::vector<float> calc_inv_scale_values(const std::vector<float>& factors);
    static std::vector<float> calc_level_sigma_sq(const std::vector<float>& factors);
    static std::vector<float> calc_inv_level_sigma_sq(const std::vector<float>& factors);
};

std::ostream& operator<<(std::ostream& os, const xfeat_params& xparam);

} // namespace feature
} // namespace stella_vslam

#endif // STELLA_VSLAM_FEATURE_XFEAT_PARAMS_H