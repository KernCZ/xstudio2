#include "stdafx.h"
#include "XmlReader.h"

namespace Library
{
   namespace IO
   {
      // -------------------------------- CONSTRUCTION --------------------------------

      /// <summary>Creates an xml reader from an input stream</summary>
      /// <param name="src">The input stream</param>
      /// <exception cref="Library::ArgumentException">Stream is not readable</exception>
      /// <exception cref="Library::ArgumentNullException">Stream is null</exception>
      /// <exception cref="Library::ComException">COM Error</exception>
      XmlReader::XmlReader(StreamPtr src) : Input(src)
      {
         REQUIRED(src);

         // Ensure stream has read access
         if (!Input->CanRead())
            throw ArgumentException(HERE, L"src", ERR_NO_READ_ACCESS);

         try
         {
            // Create document / Disable async access
            Document = IXMLDOMDocument2Ptr(CLSID_DOMDocument60);
            Document->async = VARIANT_FALSE;
         }
         catch (_com_error& ex) {
            throw ComException(HERE, ex);
         }
      }

      /// <summary>Move from an existing string reader</summary>
      /// <param name="r">The existing reader</param>
      XmlReader::XmlReader(XmlReader&& r) : Document(r.Document), Buffer(std::move(r.Buffer)), Input(std::move(r.Input))
      {
      }

      /// <summary>Closes the input stream</summary>
      XmlReader::~XmlReader()
      {
         if (Input != nullptr)      // Ensure we haven't been moved
            Input->SafeClose();
      }

      // ------------------------------- PUBLIC METHODS -------------------------------

		// ------------------------------ PROTECTED METHODS -----------------------------

      /// <summary>Parses the entire contents of the input stream</summary>
      /// <exception cref="Library::ComException">COM Error</exception>
      /// <exception cref="Library::FileFormatException">Error in the XML</exception>
      /// <exception cref="Library::InvalidOperationException">Document already loaded</exception>
      void  XmlReader::LoadDocument()
      {
         try
         {
            // Ensure we haven't already loaded
            if (Buffer != nullptr)
               throw InvalidOperationException(HERE, L"Document has already been loaded");

            // Extract stream
            Buffer = ByteArrayPtr(Input->ReadAllBytes());

            // Load/Parse file : "%s (line %d, char %d)"
            if (Document->loadXML((char*)Buffer.get()) == VARIANT_FALSE)
               throw FileFormatException(HERE, ERR_XML_PARSE_FAILED, (WCHAR*)Document->parseError->reason, Document->parseError->line, Document->parseError->linepos);
         }
         catch (_com_error& ex) {
            throw ComException(HERE, ex);
         }
      }

      /// <summary>Reads the value of an attribute</summary>
      /// <param name="node">The element containing the attribute</param>
      /// <param name="name">The attribute name</param>
      /// <returns>Attribute value</returns>
      /// <exception cref="Library::ArgumentNullException">Node is null</exception>
      /// <exception cref="Library::FileFormatException">Attribute is missing</exception>
      /// <exception cref="Library::ComException">COM Error</exception>
      wstring  XmlReader::ReadAttribute(XML::IXMLDOMNodePtr&  node, const WCHAR*  name)
      {
         REQUIRED(node);

         try
         {
            // Lookup attribute
            XML::IXMLDOMNodePtr attr = node->attributes->getNamedItem(name);

            // Ensure present : "Missing '%s' attribute on '<%s>' element"
            if (attr == nullptr)
               throw FileFormatException(HERE, ERR_XML_MISSING_ATTRIBUTE, name, (WCHAR*)node->nodeName);

            // Return text
            return (WCHAR*)attr->text;
         }
         catch (_com_error& ex) {
            throw ComException(HERE, ex);
         }
      }

      /// <summary>Verifies the name of an element</summary>
      /// <param name="node">The element</param>
      /// <param name="name">Name of the element</param>
      /// <exception cref="Library::ArgumentNullException">Node is null</exception>
      /// <exception cref="Library::FileFormatException">Missing element</exception>
      /// <exception cref="Library::ComException">COM Error</exception>
      void  XmlReader::ReadElement(XML::IXMLDOMNodePtr&  node, const WCHAR*  name)
      {
         REQUIRED(node);

         try
         {
            // Ensure name correct : "Unexpected '<%s>' element while searching for '<%s>' element"
            if (node->nodeName != _bstr_t(name))
               throw FileFormatException(HERE, ERR_XML_UNEXPECTED_ELEMENT, (WCHAR*)node->nodeName, name);
         }
         catch (_com_error& ex) {
            throw ComException(HERE, ex);
         }
      }


      /// <summary>Reads an optional attribute</summary>
      /// <param name="node">The element containing the attribute</param>
      /// <param name="name">Name of attribute</param>
      /// <returns>attribute value, or empty string</returns>
      /// <exception cref="Library::ArgumentNullException">Node is null</exception>
      /// <exception cref="Library::ComException">COM Error</exception>
      wstring  XmlReader::TryReadAttribute(XML::IXMLDOMNodePtr&  node, const WCHAR*  name)
      {
         REQUIRED(node);

         try
         {
            // Read attribute text or return empty string
            XML::IXMLDOMNodePtr attr = node->attributes->getNamedItem(name);
            return attr == NULL ? L"" : (WCHAR*)attr->text;
         }
         catch (_com_error& ex) {
            throw ComException(HERE, ex);
         }
      }

		// ------------------------------- PRIVATE METHODS ------------------------------

   }
}
