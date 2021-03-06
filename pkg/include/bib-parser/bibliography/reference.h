#ifndef TUCSE_BIB_PARSER_REFERENCE_H
#define TUCSE_BIB_PARSER_REFERENCE_H

#include <string>
#include <unordered_map>

#include "entry-type.h"
#include "field-type.h"

namespace TUCSE
{
	class Reference
	{
	public:
		Reference(std::string const &citationKey, EntryType const entryType);

		void addField(FieldType const fieldType, std::string const &value) noexcept;
		EntryType getEntryType() const noexcept;

	private:
		std::string const citationKey;
		EntryType const entryType;
		std::unordered_map<FieldType, std::string, FieldTypeHash> fields;
	};
} // namespace TUCSE

#endif // TUCSE_BIB_PARSER_REFERENCE_H