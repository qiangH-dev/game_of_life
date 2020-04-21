#pragma once

#include <algorithm>
#include <cmath>
#include <exception>
#include <stdint.h>
#include <vector>

namespace gamelife {
//坐标系 以左上角为原点
struct GridPt {
    GridPt() = default;
    GridPt(int _x , int _y ):x(_x),y(_y){}
    friend bool operator <(const GridPt& lv , const GridPt& rv);
    friend bool operator ==(const GridPt& lv , const GridPt& rv);

    constexpr bool valid() const noexcept
    {
        return x >= 0 && y >= 0;
    }

    std::vector<GridPt> around8() const
    {
        static std::vector<GridPt> Degree_8 = {
            { -1, 0 },
            { 1, 0 },
            { 0, 1 },
            { 0, -1 },
            { -1, 1 },
            { 1, 1 },
            { -1, -1 },
            { 1, -1 }
        };
        return around(Degree_8);
    }
    std::vector<GridPt> around4()
    {
        static std::vector<GridPt> Degree_4 = {
            { -1, 0 },
            { 1, 0 },
            { 0, 1 },
            { 0, -1 }
        };
        return around(Degree_4);
    }
    std::vector<GridPt> around(const std::vector<GridPt>& _degree) const
    {
        std::vector<GridPt> _res;
        _res.reserve(_degree.size());
        for (auto _offset : _degree) {
            GridPt _pt{ x + _offset.x, y + _offset.y };
            if (_pt.valid())
                _res.emplace_back(std::move(_pt));
        }
        return _res;
    }

    int x = { 0 };
    int y = { 0 };
};

bool operator <(const GridPt& lv , const GridPt& rv){
    double langle = std::atan2(lv.x , lv.y);
    double rangel = std::atan2(rv.x , rv.y);
    if(langle == langle){
        double lr = std::sqrt( (lv.x)*(lv.x) + (lv.y)*(lv.y));
        double rr = std::sqrt( (rv.x)*(rv.x) + (rv.y)*(rv.y));
        return lr < rr;
    }else{
        return langle < rangel;
    }
}

bool operator ==(const GridPt& lv , const GridPt& rv){
    return (lv.x == rv.x && lv.y == rv.y);
}

template <typename T>
class Grid {
public:
    using value_type = T;
    using contain_type = std::vector<value_type>;
    Grid() noexcept = default;
    Grid(uint32_t _cellx, uint32_t _celly , contain_type _data) noexcept
        : cellx_(_cellx)
        , celly_(_celly)
        , data_(_data)
    {
    }
    Grid(uint32_t _cellx, uint32_t _celly, std::initializer_list<contain_type> _data ):
        cellx_(_cellx),celly_(_celly),data_(_data){}
    Grid(const Grid& rhs) {
        this->cellx_ = rhs.cellx_;
        this->celly_ = rhs.celly_;
        this->data_ = rhs.data_;
    }
    Grid& operator=(const Grid& rhs) {
        rhs.swap(*this);
        return *this;
    }

    Grid(Grid&& rhs)noexcept{
        rhs.swap(*this);
    }
    Grid& operator=(Grid&& rhs)noexcept{
        rhs.swap(*this);
        return *this;
    }
    constexpr bool empty()const noexcept{
        return data_.empty();
    }
    void swap(Grid& rhs)noexcept{
        using std::swap;
        swap(cellx_ , rhs.cellx_);
        swap(celly_ , rhs.celly_);
        swap(data_,rhs.data_);
    }


    inline uint32_t x() const noexcept{ return cellx_ ;}
    inline uint32_t y() const noexcept{ return celly_ ;}
    inline contain_type data() const noexcept{ return data_; }
private:
    uint32_t cellx_ = { 0 };
    uint32_t celly_ = { 0 };
    contain_type data_;
};
}
