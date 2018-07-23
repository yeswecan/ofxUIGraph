
template< class R, class returnValue, class... Args >
class FunctionStack {
public:
    
    void operator +=(std::function<R> arg) {
        functions.push_back(arg);
    }
    
    returnValue operator() (Args... a) {
        auto size = functions.size();
        int iter = 0;
        for (auto &i: functions) {
            if (iter == size-1) {
              return i(a...);
            } else i(a...);
            iter++;
        }
    }
    
    void operator =(std::function<R> arg) {
        functions.clear();
        functions.push_back(arg);
    }
    
    std::vector<std::function<R>> functions;
};

