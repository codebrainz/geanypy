#ifndef GEANYPY_DOCUMENT_H__
#define GEANYPY_DOCUMENT_H__
#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
	PyObject_HEAD
	GeanyDocument *doc;
} Document;


PyMODINIT_FUNC init_geany_document(void);
Document *Document_create_new_from_geany_document(GeanyDocument *doc);


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* GEANYPY_DOCUMENT_H__ */
