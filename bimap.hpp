#ifndef BIMAP_HPP
# define BIMAP_HPP

# include <map>
# include <vector>

template<typename K, typename V>
class bimap
{
	private:
		std::map<K, V> _key_value;
		std::map<V, K> _value_key;
		// std::vector<K> _keys;
		// std::vector<V> _values;

	public:
		bimap() {}
		~bimap() {}

		void insert(K const &key, V const &value)
		{
			_key_value.insert(std::pair<K, V>(t, u));
			_value_key.insert(std::pair<V, K>(u, t));
			// _keys.push_back(t);
			// _values.push_back(u);
		}

		void erase(K const &key)
		{
			typename std::map<K, V>::iterator itk = _key_value.find(key);
			typename std::map<V, K>::iterator itv = _value_key.find(itk->second);
			// typename std::vector<K>::iterator itks = std::find(_keys.begin(), _keys.end(), key);
			// typename std::vector<V>::iterator itvs = std::find(_values.begin(), _values.end(), itk->second);
			_key_value.erase(itk);
			_value_key.erase(itv);
			// _keys.erase(itks);
			// _values.erase(itvs);
		}

		void erase(V const &value)
		{
			typename std::map<V, K>::iterator itv = _value_key.find(value);
			typename std::map<K, V>::iterator itk = _key_value.find(itv->second);
			_value_key.erase(itv);
			_key_value.erase(itk);
		}

		const K &get(const V &value) const
		{
			typename std::map<V, K>::const_iterator it = _value_key.find(value);
			return it->second;
		}

		const V &get(const K &key) const
		{
			typename std::map<K, V>::const_iterator it = _key_value.find(k);
			return it->second;
		}
};

#endif