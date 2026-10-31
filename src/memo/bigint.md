```cpp
#include <boost/multiprecision/cpp_int.hpp>
namespace mp = boost::multiprecision;
mp::cpp_int x = 1;
```

または

```cpp
using namespace boost::multiprecision;
cpp_int x;
x.assign("123");
```