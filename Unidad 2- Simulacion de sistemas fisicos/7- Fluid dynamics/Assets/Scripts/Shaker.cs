using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Shaker : MonoBehaviour
{
    public float amplitude = 1.0f;
    public float frequency = 1.0f;

    private Rigidbody rb;
    private Vector3 baselinePosition;
    private float t;

    // Start is called before the first frame update
    void Start()
    {
        rb = GetComponent<Rigidbody>();
        baselinePosition = rb.position;
        t = 0;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        Vector3 displacement = new Vector3(amplitude * Mathf.Sin(frequency * t), 0.0f, 0.0f);
        rb.MovePosition(baselinePosition + displacement);
        t += Time.deltaTime;
    }
}